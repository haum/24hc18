#include "scenario_base.h"
#include "gameclock.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>
#include <utility>

ScenarioBase::ScenarioBase() : m_duration{30s} {}

void ScenarioBase::set_teams(std::vector<Team *> teams) {
	m_teams = std::move(teams);
}

void ScenarioBase::set_snitch(Snitch *snitch) { m_snitch = snitch; }

void ScenarioBase::load(const char *scenario_name) {
	auto f = fopen(scenario_name, "re");
	if (f == nullptr)
		throw(std::runtime_error("scenario not found"));

	m_parser.setExecute(
	    [this](uint8_t argc, const char **argv) { processLine(argc, argv); });
	while (feof(f) == 0) {
		auto r = m_parser.read(
		    [f](char *buf, size_t len) { return fread(buf, 1, len, f); });
		if (r != LineParserError::NO_ERROR)
			std::clog << "Error while reading scenario" << std::endl;
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
		auto i = static_cast<size_t>(
		    std::distance(m_gameObjectsStorage.begin(), it));
		m_gameObjectsStorage[i] = m_gameObjectsStorage.back();
		m_gameObjectsStorage.resize(m_gameObjectsStorage.size() - 1);
	}
}

void ScenarioBase::run(GameClock &clock) {
	clock.setDuration(m_duration);
	size_t ianb = m_teams.size();
	std::vector<pollfd> fds;
	std::vector<Team *> tm;
	fds.reserve(ianb + 1);
	tm.reserve(ianb + 1);
	while (clock.running()) {
		nfds_t nbwait = 0;
		fds[nbwait].fd = m_snitch->eventFd();
		fds[nbwait].events = POLLIN;
		tm[nbwait] = nullptr;
		++nbwait;
		bool paused = true;
		bool oneShot = clock.oneShot();
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
				fds[nbwait].revents = 0;
				tm[nbwait] = t;
				++nbwait;
			}
			if (oneShot)
				t->oneShot(true);
			else if (clock.normalRun())
				t->oneShot(false);
			paused &= t->paused();
		}
		if (paused)
			breakAnchor("allTeamsArePaused");
		int respoll = poll(fds.data(), nbwait, 100);
		if (respoll < 0) {
			usleep(100'000);
		} else {
			if (respoll > 0) {
				for (nfds_t i = 0; i < nbwait; ++i) {
					if (tm[i] != nullptr) {
						if ((fds[i].revents & POLLIN) != 0)
							tm[i]->eventProcessRead();
						if ((fds[i].revents & POLLHUP) != 0)
							tm[i]->eventProcessDied();
					} else {
						if ((fds[i].revents & POLLIN) != 0)
							m_snitch->eventProcessRead();
					}
				}
			}
			uint32_t periodicCount = clock.periodicCount();
			for (uint32_t i = 0; i < periodicCount; ++i) {
				for (auto &go : m_gameObjectsStorage) {
					go->periodic();
				}
			}
		}
		clock.update();
	}
}
