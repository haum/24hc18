#include "scenario_base.h"
#include "timeutils.h"
#include <algorithm>
#include <alloca.h>
#include <cstdio>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>

ScenarioBase::ScenarioBase() : m_duration{30s} {}

void ScenarioBase::set_teams(std::vector<Team *> teams) { m_teams = teams; }

void ScenarioBase::set_snitch(Snitch *snitch) { m_snitch = snitch; }

void ScenarioBase::load(const char *scenario_name) {
	auto f = fopen(scenario_name, "r");
	if (f == nullptr)
		throw(std::runtime_error("scenario not found"));

	m_parser.setExecute(
	    [this](uint8_t argc, const char **argv) { processLine(argc, argv); });
	while (!feof(f)) {
		auto r = m_parser.read(
		    [f](char *buf, ssize_t len) { return fread(buf, 1, len, f); });
		if (r != LineParserError::NO_ERROR)
			fprintf(stderr, "Error while reading scenario\n");
	}
}

void ScenarioBase::addExistingGameObject(std::unique_ptr<GameObject> obj) {
	m_gameObjectsStorage.push_back(std::move(obj));
}

void ScenarioBase::rmGameObject(GameObject *obj) {
	auto it = std::find_if(
	    m_gameObjectsStorage.begin(), m_gameObjectsStorage.end(),
	    [obj](std::shared_ptr<GameObject> &gobj) { return gobj.get() == obj; });
	if (it != m_gameObjectsStorage.end()) {
		auto i = std::distance(m_gameObjectsStorage.begin(), it);
		m_gameObjectsStorage[i] = m_gameObjectsStorage.back();
		m_gameObjectsStorage.resize(m_gameObjectsStorage.size() - 1);
	}
}

void ScenarioBase::run() {
	auto end_tp = chr::now() + m_duration;
	auto periodic_tp = chr::now();
	size_t ianb = m_teams.size();
	auto fds = reinterpret_cast<pollfd *>(alloca((ianb + 1) * sizeof(pollfd)));
	auto tm = reinterpret_cast<Team **>(alloca((ianb + 1) * sizeof(Team *)));
	while (chr::now() < end_tp) {
		nfds_t nbwait = 0;
		fds[nbwait].fd = m_snitch->eventFd();
		fds[nbwait].events = POLLIN;
		tm[nbwait] = nullptr;
		++nbwait;
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
				if (tm[i]) {
					if (fds[i].revents & POLLIN)
						tm[i]->eventProcessRead();
					if (fds[i].revents & POLLHUP)
						tm[i]->eventProcessDied();
				} else {
					if (fds[i].revents & POLLIN)
						m_snitch->eventProcessRead();
				}
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
