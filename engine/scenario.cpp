#include "scenario.h"
#include "timeutils.h"
#include <alloca.h>
#include <poll.h>
#include <unistd.h>

Scenario::Scenario(const char * /*name*/, std::vector<Team *> teams)
	: m_teams{teams}, m_duration{30s} {
	// TODO load scenario from file
	for (size_t i = 0; i < teams.size(); ++i) {
		auto nest = std::make_unique<Nest>(*teams[i], i, i, 50);
		m_nests.push_back(nest.get());
		m_nestsStorage.push_back(std::move(nest));
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
			}
			while (chr::now() > periodic_tp) {
				periodic_tp += 100ms;
				// TODO do periodic job
			}
		}
	}
}
