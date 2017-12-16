#include "snitch.h"
#include "../scenario.h"
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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
	} while (poll(&fdp, 1, 0));

	// Write output
	m_scenario->listObjects([this](auto sgo) {
		uint32_t data[6];
		auto f = [](uint32_t *dest, float v) { ::memcpy(dest, &v, sizeof(v)); };
		auto p = [](uint32_t *dest, void *ptr) {
			uint32_t v = static_cast<uint32_t>(
				reinterpret_cast<std::uintptr_t>(ptr)); // Quite ugly
			::memcpy(dest, &v, sizeof(v));
		};

		data[0] = 0;
		f(&data[1], static_cast<float>(sgo->longitude()));
		f(&data[2], static_cast<float>(sgo->latitude()));
		f(&data[3], static_cast<float>(sgo->heading()));
		p(&data[4], sgo->teamBase());
		p(&data[5], sgo.get());
		write(m_fd, data, sizeof(data));
		return true;
	});

	uint32_t data[6] = {UINT32_MAX, 0, 0, 0, 0, 0};
	write(m_fd, data, sizeof(data));
}
