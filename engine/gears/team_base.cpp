#include "team_base.h"
#include "agent.h"
#include "rand.h"
#include <algorithm>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>

TeamBase::~TeamBase() {
	for (auto &m : m_teamManagers) {
		m_currentManager = &m;
		kill("Team is destructed");
	}
}

void TeamBase::agentAdd(Agent *agent) { m_agentsToAdd.push_back(agent); }

void TeamBase::agentRm(Agent *agent) {
	if (m_currentAgent != m_agents.end() && (*m_currentAgent) == agent)
		m_dead = true;
	m_agentsToRemove.push_back(agent);
}

int TeamBase::eventFd() const { return m_currentManager->m_fdout; }

void TeamBase::send(const char *data, size_t len) {
	if (m_log != -1) {
		char *cpy = strdup(data);
		char *ntok;
		char *narg = strtok_r(cpy, "\n", &ntok);
		while (narg != nullptr) {
			char prefix[] = "0 @ ";
			prefix[0] = static_cast<char>(
			    '0' + std::distance(m_teamManagers.begin(), m_currentManager));
			write(m_log, prefix, 4);
			write(m_log, narg, strlen(narg));
			char nl = '\n';
			write(m_log, &nl, 1);
			narg = strtok_r(nullptr, "\n", &ntok);
		}
		free(cpy);
	}
	if (m_currentManager->m_fdin > 0) {
		write(m_currentManager->m_fdin, data, len);
	}
}

void TeamBase::log(const char *msg, char prefix) {
	if ((m_log != -1) && (msg != nullptr)) {
		char prefix_str[] = {
		    static_cast<char>(
		        '0' + std::distance(m_teamManagers.begin(), m_currentManager)),
		    ' ', prefix, ' '};
		write(m_log, prefix_str, 4);
		write(m_log, msg, strlen(msg));
		char nl = '\n';
		write(m_log, &nl, 1);
	}
}

void TeamBase::kill(const char *str) {
	log(str);
	if (m_currentManager->m_pid > 0)
		::kill(m_currentManager->m_pid, SIGKILL);
	eventProcessDied();
}

void TeamBase::oneShot(bool on) {
	if (m_paused) {
		m_paused = false;
		sendPrelude();
	}
	m_oneshot = on;
}

bool TeamBase::nextAgent() {
	m_dead = false;
	if (m_currentAgent != m_agents.end())
		++m_currentAgent;
	if (m_currentAgent == m_agents.end()) {
		for (auto &agentToRemove : m_agentsToRemove) {
			auto it =
			    std::find(m_agents.begin(), m_agents.end(), agentToRemove);
			if (it != m_agents.end()) {
				*it = m_agents[m_agents.size() - 1];
				m_agents.pop_back();
			}
		}
		m_agentsToRemove.clear();
		std::copy(m_agentsToAdd.begin(), m_agentsToAdd.end(),
		          std::back_inserter(m_agents));
		m_agentsToAdd.clear();
		m_currentAgent = m_agents.begin();
		return false;
	}
	return true;
}

void TeamBase::sendPrelude() {
	if (m_oneshot || m_paused) {
		m_oneshot = false;
		m_paused = true;
		return;
	}
	std::ostringstream os;
	bool try_next = true;
	while (try_next) {
		if (m_currentAgent != m_agents.end() &&
		    (*m_currentAgent)->prelude(os)) {
			int managerIndex = std::min(
			    static_cast<int>(m_teamManagers.size() - 1),
			    static_cast<int>(random_unit() * (m_teamManagers.size())));
			m_currentManager = m_teamManagers.begin();
			std::advance(m_currentManager, managerIndex);
			const auto data = os.str();
			send(data.c_str(), data.length());
			m_stats_agents++;
			try_next = false;
		} else {
			nextAgent();
		}
	}
}

void TeamBase::eventProcessDied() {
	log("Process died");
	close(m_currentManager->m_fdin);
	m_currentManager->m_fdin = -1;
	close(m_currentManager->m_fdout);
	m_currentManager->m_fdout = -1;
	close(m_currentManager->m_fderr);
	m_currentManager->m_fderr = -1;
	m_currentManager->m_pid = -1;
	m_parser.reset();
}

void TeamBase::eventProcessRead() {
	auto r = m_parser.read([this](char *buf, size_t len) {
		return read(m_currentManager->m_fdout, buf, len);
	});
	if (r == LineParserError::LINE_TOO_LONG) {
		const char *errorstr = "Line too long, bye";
		kill(errorstr);
	} else if (r == LineParserError::READ_ERROR) {
		const char *errorstr = "Error while reading process answer";
		kill(errorstr);
	}
}

void TeamBase::processLine(uint8_t argc, const char **argv) {
	if (m_log != -1) {
		char prefix[] = "0 . ";
		prefix[0] = static_cast<char>(
		    '0' + std::distance(m_teamManagers.begin(), m_currentManager));
		write(m_log, prefix, 4);
		for (int i = 0; i < argc; ++i) {
			write(m_log, argv[i], strlen(argv[i]));
			if (i != argc - 1)
				write(m_log, " ", 1);
		}
		char nl = '\n';
		write(m_log, &nl, 1);
	}

	if ((argc == 1) && (strncmp(argv[0], "END", 3) == 0)) {
		if (!m_dead)
			(*m_currentAgent)->epilogue();
		if (!m_nokill && random_unit() < 1 / 2000.0)
			kill("Random kill");
		nextAgent();
		sendPrelude();
	} else if (!m_dead)
		(*m_currentAgent)->execute(argc, argv);
}

void TeamBase::start_subprocess() {
	int pipe0[2], pipe1[2], pipe2[2], pid;
	if (pipe(pipe0) == -1)
		goto err_pipe0;
	if (pipe(pipe1) == -1)
		goto err_pipe1;
	if (pipe(pipe2) == -1)
		goto err_pipe2;
	if ((pid = fork()) == -1)
		goto err_fork;

	if (pid != 0) { // Game manager
		m_currentManager->m_fdin = pipe0[1];
		m_currentManager->m_fdout = pipe1[0];
		m_currentManager->m_fderr = pipe2[0];
		if (m_debug)
			m_log = STDERR_FILENO;
		close(pipe0[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		m_currentManager->m_pid = pid;
		sendPrelude();
	} else { // Team manager
		dup2(pipe0[0], 0);
		dup2(pipe1[1], 1);
		dup2(pipe2[1], 2);
		close(pipe0[0]);
		close(pipe0[1]);
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		execlp("sh", "sh", "-c", m_exe.c_str(),
		       nullptr); // Here we change process
		std::clog << "Cannot start " << m_exe << ": " << strerror(errno)
		          << std::endl;
		m_currentManager->m_fdin = -1;
		m_currentManager->m_fdout = -1;
		m_currentManager->m_fderr = -1;
	}
	return;

err_fork:
	close(pipe2[1]);
	close(pipe2[0]);
err_pipe2:
	close(pipe1[1]);
	close(pipe1[0]);
err_pipe1:
	close(pipe0[1]);
	close(pipe0[0]);
err_pipe0:
	return;
}
