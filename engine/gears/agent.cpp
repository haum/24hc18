#include "agent.h"
#include "../scenario.h"
#include "utils.h"
#include <sstream>

Agent::Agent(Team &team, GameObject_t type, double latitude, double longitude,
             double heading)
    : GameObject(type, latitude, longitude, heading), m_team{team} {
	m_team.agentAdd(this);
}

Agent::~Agent() { m_team.agentRm(this); }

int32_t Agent::param_int(const char *str, bool &ok) {
	return parse_int(str, ok);
}

int32_t Agent::param_int(const char *str, bool &ok, int min, int max) {
	return parse_int(str, ok, min, max);
}

void Agent::log(const char *msg) { m_team.log(msg); }

void Agent::destroy(GameObject *obj) { m_team.scenario().rmGameObject(obj); }

void Agent::destroy() { destroy(this); }
