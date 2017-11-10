#include "scenario.h"
#include "timeutils.h"
#include <algorithm>
#include <alloca.h>
#include <cstdio>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>

#include "ant.h"

Scenario::Scenario() : m_duration{30s} {}

void Scenario::set_teams(std::vector<Team *> teams) { m_teams = teams; }

void Scenario::load(const char *scenario_name) {
	auto f = fopen(scenario_name, "r");
	if (f == nullptr)
		throw(std::runtime_error("Scenario not found"));

	m_parser.setExecute(
		[this](uint8_t argc, const char **argv) { processLine(argc, argv); });
	while (!feof(f)) {
		auto r = m_parser.read(
			[f](char *buf, ssize_t len) { return fread(buf, 1, len, f); });
		if (r != LineParserError::NO_ERROR)
			fprintf(stderr, "Error while reading scenario\n");
	}
}

void Scenario::processLine(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;

	if (!strncmp(argv[0], "NEST", 4) && argc == 5) {
		int team = atoi(argv[1]);
		if (team < 0 || static_cast<unsigned int>(team) >= m_teams.size())
			return;
		addGameObject(std::make_unique<Nest>(*m_teams[team], atof(argv[2]),
											 atof(argv[3]), atoi(argv[4])));

	} else if (!strncmp(argv[0], "ANT", 3) && argc == 5) {
		int team = atoi(argv[1]);
		if (team < 0 || static_cast<unsigned int>(team) >= m_teams.size())
			return;
		addGameObject(std::make_unique<Ant>(*m_teams[team], atof(argv[2]),
											atof(argv[3]), atof(argv[4])));

	} else if (!strncmp(argv[0], "MAXTEAMS", 8) && argc == 2) {
		auto teams = atoi(argv[1]);
		if (teams < 0)
			throw(std::runtime_error("Wrong number of teams"));
		if (static_cast<unsigned int>(teams) < m_teams.size())
			throw(
				std::runtime_error("Too many teams to play on this scenario"));
	} else if (!strncmp(argv[0], "DURATION", 8) && argc == 2) {
		auto duration = atoi(argv[1]);
		if (duration <= 0)
			throw(std::runtime_error("Invalid duration"));
		m_duration = duration * 1s;
	}
}

void Scenario::addGameObject(std::unique_ptr<GameObject> obj) {
	m_gameObjectsStorage.push_back(std::move(obj));
}

void Scenario::rmGameObject(GameObject *obj) {
	auto it = std::find_if(
		m_gameObjectsStorage.begin(), m_gameObjectsStorage.end(),
		[obj](std::shared_ptr<GameObject> &gobj) { return gobj.get() == obj; });
	if (it != m_gameObjectsStorage.end()) {
		auto i = std::distance(m_gameObjectsStorage.begin(), it);
		m_gameObjectsStorage[i] = m_gameObjectsStorage.back();
		m_gameObjectsStorage.resize(m_gameObjectsStorage.size() - 1);
	}
}

void Scenario::run() {
	auto end_tp = chr::now() + m_duration;
	auto periodic_tp = chr::now();
	size_t ianb = m_teams.size();
	auto fds = reinterpret_cast<pollfd *>(alloca(ianb * sizeof(pollfd)));
	auto tm = reinterpret_cast<Team **>(alloca(ianb * sizeof(Team *)));
	while (chr::now() < end_tp) {
		nfds_t nbwait = 0;
		for (auto *t : m_teams) {
			int fd;
			fd = t->eventFd();
			if (fd < 0) {
				t->start_subprocess();
				fd = t->eventFd();
			}
			if (fd >= 0) {
				fds[nbwait].fd = fd;
				fds[nbwait].events = POLLIN;
				tm[nbwait] = t;
				++nbwait;
			}
		}
		int respoll = poll(fds, nbwait, 100);
		if (respoll < 0) {
			usleep(100'000);
		} else {
			for (nfds_t i = 0; i < nbwait; ++i) {
				if (fds[i].revents & POLLIN)
					tm[i]->eventProcessRead();
				if (fds[i].revents & POLLHUP)
					tm[i]->eventProcessDied();
			}
			while (chr::now() > periodic_tp) {
				periodic_tp += 100ms;
				for (auto &go : m_gameObjectsStorage) {
					go->periodic();
				}
			}
		}
	}
}
