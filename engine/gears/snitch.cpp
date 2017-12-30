#include "snitch.h"
#include "../scenario.h"
#include <cstring>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../ant.h"
#include "../food.h"
#include "../nest.h"
#include "../pheromone.h"

Snitch::Snitch(const char *const host, uint16_t port, Scenario &scenario)
    : m_scenario(&scenario) {
	bool toBeClosed = true;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock != -1) {
		struct hostent *hostinfo = nullptr;
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));

		hostinfo = gethostbyname(host);
		if (hostinfo != nullptr) {
			sin.sin_addr =
			    *reinterpret_cast<struct in_addr *>(hostinfo->h_addr);
			sin.sin_port = htons(port);
			sin.sin_family = AF_INET;
			if (connect(sock, reinterpret_cast<struct sockaddr *>(&sin),
			            sizeof(struct sockaddr)) != -1) {
				m_fd = sock;
				toBeClosed = false;
			}
		}
	}
	if (toBeClosed)
		close(sock);
}

int Snitch::eventFd() const { return m_fd; }

void Snitch::eventProcessRead() {
	// Flush input
	char dummy[256];
	pollfd fdp;
	do {
		if (read(m_fd, dummy, sizeof(dummy)) != sizeof(dummy))
			break;
		fdp.fd = m_fd;
		fdp.events = POLLIN;
	} while (poll(&fdp, 1, 0) != 0);

	// Write output
	unsigned int sendIndex = 0;
	m_scenario->listObjects([this, &sendIndex](auto sgo) {
		auto f = [](uint32_t *dest, float v) { ::memcpy(dest, &v, sizeof(v)); };
		auto p = [](uint32_t *dest, void *ptr) {
			auto v = static_cast<uint32_t>(
			    reinterpret_cast<std::uintptr_t>(ptr)); // Quite ugly
			::memcpy(dest, &v, sizeof(v));
		};

		m_sendBuffer[sendIndex + 0] = UINT32_MAX - 1;
		if (sgo->category() == Pheromone::category()) {
			m_sendBuffer[sendIndex + 0] = 3;

		} else if (sgo->category() == Ant::category()) {
			m_sendBuffer[sendIndex + 0] = 0;

		} else if (sgo->category() == Food::category()) {
			m_sendBuffer[sendIndex + 0] = 1;
			if (static_cast<Food *>(sgo.get())->available() <= 0)
				return true; // Do not send

		} else if (sgo->category() == Nest::category()) {
			m_sendBuffer[sendIndex + 0] = 2;
		}

		f(&m_sendBuffer[sendIndex + 1], static_cast<float>(sgo->longitude()));
		f(&m_sendBuffer[sendIndex + 2], static_cast<float>(sgo->latitude()));
		f(&m_sendBuffer[sendIndex + 3], static_cast<float>(sgo->heading()));
		p(&m_sendBuffer[sendIndex + 4], sgo->teamBase());
		p(&m_sendBuffer[sendIndex + 5], sgo.get());
		sendIndex += 6;
		if (sendIndex >= (sizeof(m_sendBuffer) / sizeof(*m_sendBuffer))) {
			write(m_fd, m_sendBuffer, sizeof(m_sendBuffer));
			sendIndex = 0;
		}
		return true;
	});

	m_sendBuffer[sendIndex + 0] = UINT32_MAX;
	m_sendBuffer[sendIndex + 1] = m_sendBuffer[sendIndex + 2] = 0;
	m_sendBuffer[sendIndex + 3] = m_sendBuffer[sendIndex + 4] = 0;
	m_sendBuffer[sendIndex + 5] = 0;
	sendIndex += 6;
	write(m_fd, m_sendBuffer, sendIndex * sizeof(*m_sendBuffer));
}
