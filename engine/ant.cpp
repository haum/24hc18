#include "ant.h"
#include "gears/rand.h"
#include "pheromone.h"
#include "scenario.h"
#include <cmath>

const GameObjectCategory Ant::s_category{"ANT"};

Ant::Ant(Team &team, double latitude, double longitude, double heading,
		 uint8_t ant_type, uint8_t memory1, uint8_t memory2)
	: Agent{team, Ant::category(), latitude, longitude, heading},
	  m_ant_type{ant_type}, m_memory{memory1, memory2} {}

void Ant::turnLeft() { m_heading = fmod(m_heading + 2 * M_PI / 36, 2 * M_PI); }

void Ant::turnRight() { m_heading = fmod(m_heading - 2 * M_PI / 36, 2 * M_PI); }

void Ant::walk() {
	const double dm = 2 * M_PI / 10000;
	double dh = (random_angle() - M_PI) / 36;
	m_heading = fmod(m_heading + dh, 2 * M_PI);
	m_latitude = fmod(m_latitude + dm * cos(m_heading), 2 * M_PI);
	m_longitude = fmod(m_longitude + dm * sin(m_heading), 2 * M_PI);
}

bool Ant::prelude(std::ostream &os) {
	os << "BEGIN ANT\n";
	os << "TYPE " << static_cast<int>(m_ant_type) << '\n';
	os << "MEMORY " << static_cast<int>(m_memory[0]) << ' '
	   << static_cast<int>(m_memory[1]) << '\n';
	os << "END\n";
	os.flush();
	m_actionState = ACTION_FREE;
	return true;
}

void Ant::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;
	if (!strncmp(argv[0], "MEMORY", 7) && argc == 3) {
		bool ok1, ok2;
		int m0 = param_int(argv[1], ok1, 0, 255);
		int m1 = param_int(argv[2], ok2, 0, 255);
		if (ok1 && ok2) {
			m_memory[0] = static_cast<uint8_t>(m0);
			m_memory[1] = static_cast<uint8_t>(m1);
		}

	} else if (!strncmp(argv[0], "SUICIDE", 7) && argc == 1) {
		destroy();
	} else if (!strncmp(argv[0], "PUT_PHEROMONE", 14) && argc == 1) {
		team().scenario().addGameObject<Pheromone>(12, 23, team(), 3);
	} else if (!strncmp(argv[0], "WALK", 5) && argc == 1) {
		if (m_actionState == ACTION_FREE) {
			walk();
			m_actionState = ACTION_MADE;
		} else {
			m_actionState = ACTION_MULTIPLE;
		}

	} else if (!strncmp(argv[0], "TURNLEFT", 9) && argc == 1) {
		if (m_actionState == ACTION_FREE) {
			turnLeft();
			m_actionState = ACTION_MADE;
		} else {
			m_actionState = ACTION_MULTIPLE;
		}

	} else if (!strncmp(argv[0], "TURNRIGHT", 10) && argc == 1) {

		if (m_actionState == ACTION_FREE) {
			turnRight();
			m_actionState = ACTION_MADE;
		} else {
			m_actionState = ACTION_MULTIPLE;
		}
	}
}
