#include "team_base.h"
#include "agent.h"
#include "rand.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <unistd.h>

void TeamBase::agentAdd(Agent *agent) { m_agents.push_back(agent); }

void TeamBase::agentRm(Agent *agent) {
	if ((*m_currentAgent) == agent)
		m_dead = true;
	m_agentsToRemove.push_back(agent);
}

int TeamBase::eventFd() { return m_fdout; }

void TeamBase::send(const char *data, size_t len) {
	if (m_log != -1) {
		char *cpy = strdup(data);
		char *ntok;
		char *narg = strtok_r(cpy, "\n", &ntok);
		while (narg != nullptr) {
			write(m_log, "@ ", 2);
			write(m_log, narg, strlen(narg));
			char nl = '\n';
			write(m_log, &nl, 1);
			narg = strtok_r(NULL, "\n", &ntok);
		}
		free(cpy);
	}
	if (m_fdin > 0) {
		write(m_fdin, data, len);
	}
}

void TeamBase::kill(const char *str) {
	if (m_log != -1 && str) {
		write(m_log, str, strlen(str));
		char nl = '\n';
		write(m_log, &nl, 1);
	}
	::kill(m_pid, SIGKILL);
	eventProcessDied();
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
		m_currentAgent = m_agents.begin();
		return false;
	}
	return true;
}

bool TeamBase::sendPrelude() {
	std::ostringstream os;
	if (m_currentAgent != m_agents.end() && (*m_currentAgent)->prelude(os)) {
		const auto data = os.str();
		send(data.c_str(), data.length());
		return true;
	}
	return false;
}

void TeamBase::eventProcessDied() {
	write(m_log, "! Process died\n", 15);
	close(m_fdin);
	m_fdin = -1;
	close(m_fdout);
	m_fdout = -1;
	close(m_fderr);
	m_fderr = -1;
	m_pid = -1;
	m_parser.reset();
}

void TeamBase::eventProcessRead() {
	auto r = m_parser.read(
		[this](char *buf, ssize_t len) { return read(m_fdout, buf, len); });
	if (r == LineParserError::LINE_TOO_LONG) {
		const char *errorstr = "! Line too long, bye";
		kill(errorstr);
	} else if (r == LineParserError::READ_ERROR) {
		const char *errorstr = "! Error while reading process answer";
		kill(errorstr);
	}
}

void TeamBase::processLine(uint8_t argc, const char **argv) {
	if (m_log != -1) {
		write(m_log, ". ", 2);
		for (int i = 0; i < argc; ++i) {
			write(m_log, argv[i], strlen(argv[i]));
			if (i != argc - 1)
				write(m_log, " ", 1);
		}
		char nl = '\n';
		write(m_log, &nl, 1);
	}

	if (!m_dead)
		(*m_currentAgent)->execute(argc, argv);
	if (argc == 1 && !strncmp(argv[0], "END", 3)) {
		if (!m_dead)
			(*m_currentAgent)->epilogue();
		bool trynext = true;
		while (trynext) {
			if (nextAgent()) {
				trynext = !sendPrelude();
			} else {
				if (random_unit() < 1 / 15.0) {
					kill("! Random kill");
					trynext = false;
				} else {
					trynext = !sendPrelude();
				}
			}
		}
	}
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

	if (pid) { // Game manager
		m_fdin = pipe0[1];
		m_fdout = pipe1[0];
		m_fderr = pipe2[0];
		if (m_debug)
			m_log = STDERR_FILENO;
		close(pipe0[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		m_pid = pid;
		m_currentAgent = m_agents.begin();
		while (!sendPrelude() && nextAgent()) {
			// Nothing to do
		}
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
		std::cerr << "Cannot start " << m_exe << ": " << strerror(errno)
				  << std::endl;
		m_fdin = -1;
		m_fdout = -1;
		m_fderr = -1;
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
