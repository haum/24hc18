#include "scenario.h"
#include "gears/gameclock.h"
#include "gears/utils.h"
#include <climits>
#include <cmath>

#include "ant.h"
#include "food.h"
#include "nest.h"
#include "pheromone.h"

namespace {
constexpr int MIN_TEAM = 0;
constexpr int MAX_TEAM = 20;
constexpr int MIN_U8 = 0;
constexpr int MAX_U8 = 255;
constexpr int MIN_LATITUDE = -90;
constexpr int MAX_LATITUDE = 90;
constexpr int MIN_LONGITUDE = -180;
constexpr int MAX_LONGITUDE = 180;
constexpr int MIN_HEADING = -180;
constexpr int MAX_HEADING = 180;
} // namespace

void Scenario::actionMaxteams(uint8_t teams) {
	if (static_cast<unsigned int>(teams) < m_teams.size())
		throw(std::runtime_error("Too many teams to play on this scenario"));
}

void Scenario::actionDuration(int s) { m_duration = s * 1s; }

void Scenario::actionNestPosition(uint8_t team, int latitude, int longitude) {
	auto *nest = findNest(team);
	if (nest != nullptr) {
		nest->setPosition(latitude * M_PI / 180, longitude * M_PI / 180);
	}
}

void Scenario::actionNestPopulation(uint8_t team, uint8_t type, int nb) {
	auto *nest = findNest(team);
	if (nest != nullptr) {
		nest->setPopulation(type, nb);
	}
}

void Scenario::actionNestFood(uint8_t team, int amount) {
	auto *nest = findNest(team);
	if (nest != nullptr) {
		nest->setFood(amount);
	}
}

void Scenario::actionAnt(uint8_t team, uint8_t type, int latitude,
                         int longitude, int heading, int stamina,
                         uint8_t memory1, uint8_t memory2) {
	auto teamPtr = findTeam(team);
	if (teamPtr != nullptr)
		addGameObject<Ant>(*teamPtr, stamina, latitude * M_PI / 180,
		                   longitude * M_PI / 180, heading * M_PI / 180, type,
		                   0, memory1, memory2);
}

void Scenario::actionPheromone(uint8_t team, uint8_t type, int latitude,
                               int longitude) {
	auto teamPtr = findTeam(team);
	if (teamPtr != nullptr)
		addGameObject<Pheromone>(latitude * M_PI / 180, longitude * M_PI / 180,
		                         *teamPtr, type);
}

void Scenario::actionFood(int latitude, int longitude, int initialValue,
                          float chargeRate, int chargeMax, int totalChargeMax,
                          bool deadIfEmpty) {
	addGameObject<Food>(latitude * M_PI / 180, longitude * M_PI / 180,
	                    initialValue, chargeRate, chargeMax, totalChargeMax,
	                    deadIfEmpty);
}

Team *Scenario::findTeam(uint8_t team) {
	if (team >= m_teams.size())
		return nullptr;
	return m_teams[static_cast<size_t>(team)];
}

Nest *Scenario::findNest(uint8_t team) {
	Nest *ret = nullptr;
	auto teamPtr = findTeam(team);
	if (teamPtr == nullptr)
		return ret;
	listObjects([&ret, teamPtr](auto sgo) {
		if (sgo->category() == Nest::category()) {
			auto *nest = static_cast<Nest *>(sgo.get());
			if (&nest->team() == teamPtr) {
				ret = nest;
				return false;
			}
		}
		return true;
	});
	if (ret == nullptr) {
		ret = addGameObject<Nest>(*teamPtr, 0, 0);
	}
	return ret;
}

void Scenario::processLine(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;

	bool ok;
	if ((argc == 9) && (strncmp(argv[0], "ANT", 4) == 0)) {
		auto team = parse_int(argv[1], ok, MIN_TEAM, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid team number"));
		auto type = parse_int(argv[2], ok, MIN_U8, MAX_U8);
		if (!ok)
			throw(std::runtime_error("Invalid type number"));
		auto latitude = parse_int(argv[3], ok, MIN_LATITUDE, MAX_LATITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid latitude"));
		auto longitude = parse_int(argv[4], ok, MIN_LONGITUDE, MAX_LONGITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid longitude"));
		auto heading = parse_int(argv[5], ok, MIN_HEADING, MAX_HEADING);
		if (!ok)
			throw(std::runtime_error("Invalid heading"));
		auto stamina =
		    parse_int(argv[6], ok, 0, std::numeric_limits<int>::max());
		auto memory1 = parse_int(argv[7], ok, MIN_U8, MAX_U8);
		auto memory2 = parse_int(argv[8], ok, MIN_U8, MAX_U8);
		actionAnt(static_cast<uint8_t>(team), static_cast<uint8_t>(type),
		          latitude, longitude, heading, stamina,
		          static_cast<uint8_t>(memory1), static_cast<uint8_t>(memory2));

	} else if ((argc == 5) && (strncmp(argv[0], "PHEROMONE", 10) == 0)) {
		auto team = parse_int(argv[1], ok, MIN_TEAM, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid team number"));
		auto type = parse_int(argv[2], ok, MIN_U8, MAX_U8);
		if (!ok)
			throw(std::runtime_error("Invalid type number"));
		auto latitude = parse_int(argv[3], ok, MIN_LATITUDE, MAX_LATITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid latitude"));
		auto longitude = parse_int(argv[4], ok, MIN_LONGITUDE, MAX_LONGITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid longitude"));
		actionPheromone(static_cast<uint8_t>(team), static_cast<uint8_t>(type),
		                latitude, longitude);

	} else if ((argc == 8) && (strncmp(argv[0], "FOOD", 5) == 0)) {
		auto latitude = parse_int(argv[1], ok, MIN_LATITUDE, MAX_LATITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid latitude"));
		auto longitude = parse_int(argv[2], ok, MIN_LONGITUDE, MAX_LONGITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid longitude"));
		auto initialValue =
		    parse_int(argv[3], ok, 0, std::numeric_limits<int>::max());
		if (!ok)
			throw(std::runtime_error("Invalid initial value"));
		float chargeRate = parse_float(argv[4], ok, 0, 10000);
		if (!ok)
			throw(std::runtime_error("Invalid charge rate"));
		auto chargeMax =
		    parse_int(argv[5], ok, 0, std::numeric_limits<int>::max());
		if (!ok)
			throw(std::runtime_error("Invalid maximal charge"));
		auto totalChargeMax =
		    parse_int(argv[6], ok, 0, std::numeric_limits<int>::max());
		if (!ok)
			throw(std::runtime_error("Invalid total maximal charge"));
		auto deadIfEmpty = parse_int(argv[7], ok, 0, 1);
		if (!ok)
			throw(std::runtime_error("Invalid total maximal charge"));
		actionFood(latitude, longitude, initialValue, chargeRate, chargeMax,
		           totalChargeMax, deadIfEmpty == 1);

	} else if ((argc == 4) && (strncmp(argv[0], "NEST_POSITION", 14) == 0)) {
		auto team = parse_int(argv[1], ok, MIN_TEAM, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid team number"));
		auto latitude = parse_int(argv[2], ok, MIN_LATITUDE, MAX_LATITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid latitude"));
		auto longitude = parse_int(argv[3], ok, MIN_LONGITUDE, MAX_LONGITUDE);
		if (!ok)
			throw(std::runtime_error("Invalid longitude"));
		actionNestPosition(static_cast<uint8_t>(team), latitude, longitude);

	} else if ((argc == 3) && (strncmp(argv[0], "NEST_FOOD", 10) == 0)) {
		auto team = parse_int(argv[1], ok, MIN_TEAM, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid team number"));
		auto amount =
		    parse_int(argv[2], ok, 0, std::numeric_limits<int>::max());
		if (!ok)
			throw(std::runtime_error("Invalid amount"));
		actionNestFood(static_cast<uint8_t>(team), amount);

	} else if ((argc == 4) && (strncmp(argv[0], "NEST_POPULATION", 16) == 0)) {
		auto team = parse_int(argv[1], ok, MIN_TEAM, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid team number"));
		auto type = parse_int(argv[2], ok, MIN_U8, MAX_U8);
		if (!ok)
			throw(std::runtime_error("Invalid type number"));
		auto nb = parse_int(argv[3], ok, 0, std::numeric_limits<int>::max());
		if (!ok)
			throw(std::runtime_error("Invalid nb"));
		actionNestPopulation(static_cast<uint8_t>(team),
		                     static_cast<uint8_t>(type), nb);

	} else if ((argc == 2) && (strncmp(argv[0], "MAXTEAMS", 9) == 0)) {
		auto teams = parse_int(argv[1], ok, 1, MAX_TEAM);
		if (!ok)
			throw(std::runtime_error("Invalid teams number"));
		actionMaxteams(static_cast<uint8_t>(teams));

	} else if ((argc == 2) && (strncmp(argv[0], "DURATION", 9) == 0)) {
		auto duration = parse_int(argv[1], ok, 1, INT_MAX);
		if (!ok)
			throw(std::runtime_error("Invalid duration"));
		actionDuration(duration);
	}
}
