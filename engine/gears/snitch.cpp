#include "snitch.h"
#include "../scenario.h"
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

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
		char cat[20];
		snprintf(cat, sizeof(cat), "%s\n",
				 sgo->category()->name().c_str()); // Create placeholder answer
												   // to be replaced when
												   // protocol will be defined
		write(m_fd, cat, strlen(cat));
		return true;
	});
}
