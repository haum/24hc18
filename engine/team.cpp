#include "team.h"
#include <iostream>

void Team::printStats() {
	std::cout << "Team " << m_exe << '\n';
	std::cout << m_agents.size() << " agents alive" << '\n';
	std::cout << std::flush;
}
