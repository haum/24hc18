#include "ant.h"
#include "gears/rand.h"
#include "pheromone.h"
#include "scenario.h"
#include <cmath>

const GameObjectCategory Ant::s_category{"ANT"};

Ant::Ant(Team &team, int life, double latitude, double longitude,
		 double heading, uint8_t ant_type, uint8_t memory1, uint8_t memory2)
	: Agent{team, Ant::category(), latitude, longitude, heading}, m_life{life},
	  m_ant_type{ant_type}, m_memory{memory1, memory2} {
	m_teamBase = &team;
}

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
	size_t gameObjectId = 0;
	team().resetIds();
	team().scenario().listObjects([&gameObjectId, this, &os](auto sgo) {
		if (this != sgo.get()) {
			if (sgo->distance(*this) <= 0.9) {
				if (sgo->distance(*this) <= 0.4) {
					os << "INTERACTABLE " << sgo->category()->name();
					gameObjectId = team().addId(sgo.get());
				} else {
					os << "VISION " << sgo->category()->name();
					gameObjectId = team().addId(sgo.get());
				}
				if (sgo->category() == Pheromone::category()) {
					auto *pheromone = static_cast<Pheromone *>(sgo.get());
					os << " " << pheromone->type() << " " << gameObjectId
					   << "\n";
				} else if (sgo->category() == Ant::category()) {
					auto *ant = static_cast<Ant *>(sgo.get());
					bool team;
					if (&this->team() == &ant->team()) {
						team = true;
					} else {
						team = false;
					}
					os << " " << team << " " << ant->life() << " "
					   << gameObjectId << "\n";
				} else if (sgo->category() == Nest::category()) {
					auto *nest = static_cast<Nest *>(sgo.get());
					bool team;
					if (&this->team() == &nest->team()) {
						team = true;
					} else {
						team = false;
					}
					os << " " << team << " " << gameObjectId << "\n";
				}
			}
		}
		return true;
	});

	os << "END\n";
	os.flush();
	m_exclusiveDone = false;
	return true;
}

void Ant::invalidAction() {
	log("Invalid action, kill ant");
	destroy();
}

bool Ant::actionPrelude(int cost, ActionType type, bool valid) {
	m_life -= cost;
	if (m_life < 0) {
		m_life = 0;
		return false;
	}
	if (type == EXCLUSIVE) {
		if (m_exclusiveDone) {
			invalidAction();
			return false;
		} else {
			m_exclusiveDone = true;
		}

	} else if (type == ALWAYS_ALLOWED) {
		// Nothing to do
	}
	return valid;
}

void Ant::actionMemory(bool valid, uint8_t m0, uint8_t m1) {
	if (!actionPrelude(0, ALWAYS_ALLOWED, valid))
		return;
	m_memory[0] = m0;
	m_memory[1] = m1;
}

void Ant::actionSuicide(bool valid) {
	if (!actionPrelude(0, ALWAYS_ALLOWED, valid))
		return;
	destroy();
}

void Ant::actionPutPheromone(bool valid, uint8_t type) {
	if (!actionPrelude(0, EXCLUSIVE, valid))
		return;
	team().scenario().addGameObject<Pheromone>(this->longitude(),
											   this->latitude(), team(), type);
}

void Ant::actionChangePheromone(bool valid, int id, uint8_t type) {
	if (!actionPrelude(0, EXCLUSIVE, valid))
		return;
	if (id <= 0) {
		invalidAction();
		return;
	}
	size_t index = static_cast<size_t>(id - 1);
	if (index >= team().getIds().size()) {
		invalidAction();
		return;
	}

	GameObject *ptr = team().getIds()[index];
	team().scenario().listObjects([this, ptr, type](auto sgo) {
		if (ptr == sgo.get()) {
			if (sgo->category() == Pheromone::category()) {
				auto *pheromone = static_cast<Pheromone *>(sgo.get());
				pheromone->setType(type);
			} else {
				invalidAction();
			}
			return false;
		}
		return true;
	});
}

void Ant::actionWalk(bool valid) {
	if (!actionPrelude(0, EXCLUSIVE, valid))
		return;
	walk();
}

void Ant::actionTurnLeft(bool valid) {
	if (!actionPrelude(0, EXCLUSIVE, valid))
		return;
	turnLeft();
}

void Ant::actionTurnRight(bool valid) {
	if (!actionPrelude(0, EXCLUSIVE, valid))
		return;
	turnRight();
}

void Ant::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;
	if (!strncmp(argv[0], "MEMORY", 7) && argc == 3) {
		bool ok1, ok2;
		int m0 = param_int(argv[1], ok1, 0, 255);
		int m1 = param_int(argv[2], ok2, 0, 255);
		actionMemory(ok1 && ok2, static_cast<uint8_t>(m0),
					 static_cast<uint8_t>(m1));

	} else if (!strncmp(argv[0], "SUICIDE", 8) && argc == 1) {
		actionSuicide(true);

	} else if (!strncmp(argv[0], "PUT_PHEROMONE", 14) && argc == 2) {
		bool ok = false;
		int type = param_int(argv[1], ok, 0, 255);
		actionPutPheromone(ok, static_cast<uint8_t>(type));

	} else if (!strncmp(argv[0], "CHANGE_PHEROMONE", 17) && argc == 3) {
		bool ok1, ok2;
		int id = param_int(argv[1], ok1, 0, 255);
		int type = param_int(argv[2], ok2, 0, 255);
		actionChangePheromone(ok1 && ok2, id, static_cast<uint8_t>(type));

	} else if (!strncmp(argv[0], "WALK", 5) && argc == 1) {
		actionWalk(true);

	} else if (!strncmp(argv[0], "TURNLEFT", 9) && argc == 1) {
		actionTurnLeft(true);

	} else if (!strncmp(argv[0], "TURNRIGHT", 10) && argc == 1) {
		actionTurnRight(true);

	} else {
		invalidAction();
	}
}
