#include "scenario.h"
#include "gears/timeutils.h"

#include "ant.h"
#include "nest.h"

void Scenario::processLine(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;

	if (!strncmp(argv[0], "NEST", 4) && argc == 5) {
		int team = atoi(argv[1]);
		if (team < 0 || static_cast<unsigned int>(team) >= m_teams.size())
			return;
		addGameObject<Nest>(*m_teams[team], atof(argv[2]), atof(argv[3]),
							atoi(argv[4]));

	} else if (!strncmp(argv[0], "ANT", 3) && argc == 5) {
		int team = atoi(argv[1]);
		if (team < 0 || static_cast<unsigned int>(team) >= m_teams.size())
			return;
		addGameObject<Ant>(*m_teams[team], atof(argv[2]), atof(argv[3]),
						   atof(argv[4]));

	} else if (!strncmp(argv[0], "MAXTEAMS", 8) && argc == 2) {
		auto teams = atoi(argv[1]);
		if (teams < 0)
			throw(std::runtime_error("Wrong number of teams"));
		if (static_cast<unsigned int>(teams) < m_teams.size())
			throw(
				std::runtime_error("Too many teams to play on this scenario"));
	} else if (!strncmp(argv[0], "DURATION", 8) && argc == 2) {
		auto duration = atoi(argv[1]);
		if (duration <= 0)
			throw(std::runtime_error("Invalid duration"));
		m_duration = duration * 1s;
	}
}
