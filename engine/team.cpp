#include "team.h"
#include <algorithm>

void Team::agentAdd(Agent *agent) { m_agents.push_back(agent); }

void Team::agentRm(Agent *agent) {
	auto it = std::find(m_agents.begin(), m_agents.end(), agent);
	if (it != m_agents.end()) {
		*it = m_agents[m_agents.size() - 1];
		m_agents.pop_back();
	}
}
