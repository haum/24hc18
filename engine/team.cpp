#include "team.h"
#include "nest.h"
#include "scenario.h"
#include <iostream>

void Team::printStats() {
	Nest *nest = nullptr;
	scenario().listObjects([&nest, this](auto sgo) {
		if (sgo->category() == Nest::category()) {
			auto *n = static_cast<Nest *>(sgo.get());
			if (&n->team() == this) {
				nest = n;
				return false;
			}
		}
		return true;
	});

	std::cout << "Team executable: " << m_exe << '\n';
	std::cout << "Executed strategies: " << m_stats_agents << '\n';
	std::cout << "Strategy balances: " << m_stats_balancing << '\n';
	std::cout << "Random kills: " << m_stats_kills << '\n';
	std::cout << "Alive agents: " << m_agents.size() << '\n';
	if (nest) {
		size_t score = nest->foodAmount() + nest->antsCount();
		score += m_agents.size() - 1;

		std::cout << "Food amount in nest: " << nest->foodAmount() << '\n';
		std::cout << "Ants inside nest: " << nest->antsCount() << '\n';
		std::cout << "Score: " << score << '\n';
	}
	std::cout << std::flush;
}
