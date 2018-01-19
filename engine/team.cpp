#include "team.h"
#include "nest.h"
#include "scenario.h"
#include <iostream>

Nest *Team::nest() {
	Nest *nest_ret = nullptr;
	scenario().listObjects([&nest_ret, this](auto sgo) {
		if (sgo->category() == Nest::category()) {
			auto *n = static_cast<Nest *>(sgo.get());
			if (&n->team() == this) {
				nest_ret = n;
				return false;
			}
		}
		return true;
	});
	return nest_ret;
}

size_t Team::score() {
	Nest *n = nest();
	size_t score = 0;
	if (n) {
		score += m_agents.size() - 1 + n->antsCount();
		if (score > 0)
			score += n->foodAmount();
	}
	return score;
}

void Team::printStats(bool apioutput, size_t score_max) {
	Nest *n = nest();
	std::cout << "Team executable: " << m_exe << '\n';
	std::cout << "Executed strategies: " << m_stats_agents << '\n';
	std::cout << "Strategy balances: " << m_stats_balancing << '\n';
	std::cout << "Random kills: " << m_stats_kills << '\n';
	std::cout << "Alive agents: " << m_agents.size() << '\n';
	if (n) {
		auto s = score();
		std::cout << "Food amount in nest: " << n->foodAmount() << '\n';
		std::cout << "Ants inside nest: " << n->antsCount() << '\n';
		std::cout << "Score: " << s << '\n';
		if (apioutput) {
			if (s == score_max && score_max != 0)
				s = 6;
			else if (s != 0)
				s = 4;
			std::cout << "API:: " << s << '\n';
		}
	}
	std::cout << std::flush;
}
