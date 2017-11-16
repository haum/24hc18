#include "agent.h"
#include "../scenario.h"
#include <sstream>

Agent::Agent(Team &team, GameObject_t type, double latitude, double longitude,
			 double heading)
	: GameObject(type, latitude, longitude, heading), m_team{team} {
	m_team.agentAdd(this);
}

Agent::~Agent() { m_team.agentRm(this); }

int32_t Agent::param_int(const char *str, bool &ok) {
	char *endptr;
	long int value = strtol(str, &endptr, 0);
	ok = (*endptr == 0 && endptr != str && value <= INT32_MAX &&
		  value >= INT32_MIN);
	return static_cast<int>(value);
}

int32_t Agent::param_int(const char *str, bool &ok, int min, int max) {
	int ret = param_int(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}

void Agent::log(const char *msg) { m_team.log(msg); }

void Agent::destroy(GameObject *obj) { m_team.scenario().rmGameObject(obj); }

void Agent::destroy() { destroy(this); }
