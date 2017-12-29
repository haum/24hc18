#include "ant.h"
#include "food.h"
#include "gears/rand.h"
#include "pheromone.h"
#include "scenario.h"
#include <cmath>

namespace {
constexpr const double WALK_DISTANCE = 2 * M_PI / 10000;
constexpr const double NEAR_DISTANCE = 3 * WALK_DISTANCE;
constexpr const double FAR_DISTANCE = 10 * WALK_DISTANCE;
static_assert(WALK_DISTANCE <= NEAR_DISTANCE,
              "WALK_DISTANCE should be smaller than NEAR_DISTANCE");
static_assert(NEAR_DISTANCE < FAR_DISTANCE,
              "NEAR_DISTANCE should be nearer than FAR_DISTANCE");
} // namespace

const GameObjectCategory Ant::s_category{"ANT"};

Ant::Ant(Team &team, int stamina, double latitude, double longitude,
         double heading, uint8_t ant_type, uint8_t memory1, uint8_t memory2)
    : Agent{team, Ant::category(), latitude, longitude, heading},
      m_stamina{stamina}, m_stock{0}, m_attacked{false},
      m_ant_type{ant_type}, m_memory{memory1, memory2} {
	m_teamBase = &team;
}

bool Ant::prelude(std::ostream &os) {
	os << "BEGIN ANT\n";
	os << "TYPE " << static_cast<int>(m_ant_type) << '\n';
	os << "MEMORY " << static_cast<int>(m_memory[0]) << ' '
	   << static_cast<int>(m_memory[1]) << '\n';
	if (m_attacked) {
		os << "ATTACKED\n";
		m_attacked = false;
	}
	os << "STAMINA " << static_cast<int>(m_stamina) << '\n';
	os << "STOCK " << static_cast<int>(m_stock) << '\n';
	team().resetIds();
	team().scenario().listObjects([this, &os](auto sgo) {
		if (this != sgo.get()) {
			if (sgo->distance(*this) <= FAR_DISTANCE) {
				size_t gameObjectId = team().addId(sgo.get());
				const char *zoneTxt =
				    (sgo->distance(*this) <= NEAR_DISTANCE ? " NEAR" : " FAR");

				if (sgo->category() == Pheromone::category()) {
					auto *pheromone = static_cast<Pheromone *>(sgo.get());
					if (&pheromone->team() == &team()) {
						os << "SEE_PHEROMONE";
						os << ' ' << gameObjectId;
						os << zoneTxt;
						os << ' ' << static_cast<int>(pheromone->type());
						os << '\n';
					}

				} else if (sgo->category() == Ant::category()) {
					auto *ant = static_cast<Ant *>(sgo.get());
					bool ownTeam = (&this->team() == &ant->team());
					os << "SEE_ANT";
					os << ' ' << gameObjectId;
					os << zoneTxt;
					os << (ownTeam ? " FRIEND" : " ENEMY");
					os << ' ' << ant->stamina();
					os << '\n';

				} else if (sgo->category() == Nest::category()) {
					auto *nest = static_cast<Nest *>(sgo.get());
					bool ownTeam = (&this->team() == &nest->team());
					os << "SEE_NEST";
					os << ' ' << gameObjectId;
					os << zoneTxt;
					os << (ownTeam ? " FRIEND" : " ENEMY");
					os << '\n';

				} else if (sgo->category() == Food::category()) {
					auto *food = static_cast<Food *>(sgo.get());
					int amount = food->available();
					if (amount > 0) {
						os << "SEE_FOOD";
						os << ' ' << gameObjectId;
						os << zoneTxt;
						os << ' ' << amount;
						os << '\n';
					}
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
	m_stamina -= cost;
	if (m_stamina < 0) {
		destroy();
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

GameObject *Ant::getObjectById(int id, bool &invalid) {
	if (id <= 0) {
		invalid = true;
		return nullptr;
	}
	size_t index = static_cast<size_t>(id - 1);
	if (index >= team().getIds().size()) {
		invalid = true;
		return nullptr;
	}
	invalid = false;
	auto *ptr = team().getIds()[index];
	bool found = false;
	team().scenario().listObjects([ptr, &found](auto sgo) {
		if (ptr == sgo.get()) {
			found = true;
			return false;
		}
		return true;
	});
	if (!found)
		return nullptr;
	return ptr;
}

void Ant::actionSetMemory(bool valid, uint8_t m0, uint8_t m1) {
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
	if (!actionPrelude(3, EXCLUSIVE, valid))
		return;
	team().scenario().addGameObject<Pheromone>(this->latitude(),
	                                           this->longitude(), team(), type);
}

void Ant::actionChangePheromone(bool valid, uint8_t type, int id) {
	if (!actionPrelude(2, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectById(id, invalid);
	if (invalid || ptr->category() != Pheromone::category()) {
		invalidAction();
		return;
	}

	if (ptr && ptr->distance(*this) <= NEAR_DISTANCE) {
		auto *pheromone = static_cast<Pheromone *>(ptr);
		pheromone->setType(type);
	}
}

void Ant::actionRechargePheromone(bool valid, int id) {
	if (!actionPrelude(1, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectById(id, invalid);
	if (invalid || ptr->category() != Pheromone::category()) {
		invalidAction();
		return;
	}

	if (ptr && ptr->distance(*this) <= NEAR_DISTANCE) {
		auto *pheromone = static_cast<Pheromone *>(ptr);
		pheromone->setLife(100);
	}
}

void Ant::actionAttack(bool valid, int id) {
	if (!actionPrelude(2, EXCLUSIVE, valid)) {
		return;
	}

	bool invalid;
	GameObject *ptr = getObjectById(id, invalid);
	if (invalid || ptr->category() != Ant::category()) {
		invalidAction();
		return;
	}

	if (ptr && ptr->distance(*this) <= NEAR_DISTANCE) {
		auto *ant = static_cast<Ant *>(ptr);
		ant->m_stamina -= 3;
		ant->m_attacked = true;
		if (ant->m_stamina < 0)
			ant->destroy();
	}
}

void Ant::actionExplore(bool valid) {
	if (!actionPrelude(1, EXCLUSIVE, valid))
		return;
	double dh = (random_angle() - M_PI) / 36;
	m_heading = fmod(m_heading + dh, 2 * M_PI);
	moveDistance(WALK_DISTANCE);
}

void Ant::actionTurn(bool valid, int angle) {
	if (!actionPrelude(1, EXCLUSIVE, valid))
		return;
	m_heading = fmod(m_heading + angle * M_PI / 180, 2 * M_PI);
}

void Ant::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;
	if (!strncmp(argv[0], "SET_MEMORY", 11) && argc == 3) {
		bool ok1, ok2;
		int m0 = param_int(argv[1], ok1, 0, 255);
		int m1 = param_int(argv[2], ok2, 0, 255);
		actionSetMemory(ok1 && ok2, static_cast<uint8_t>(m0),
		                static_cast<uint8_t>(m1));

	} else if (!strncmp(argv[0], "SUICIDE", 8) && argc == 1) {
		actionSuicide(true);

	} else if (!strncmp(argv[0], "PUT_PHEROMONE", 14) && argc == 2) {
		bool ok = false;
		int type = param_int(argv[1], ok, 0, 255);
		actionPutPheromone(ok, static_cast<uint8_t>(type));

	} else if (!strncmp(argv[0], "CHANGE_PHEROMONE", 17) && argc == 3) {
		bool ok1, ok2;
		int type = param_int(argv[1], ok1, 0, 255);
		int id = param_int(argv[2], ok2, 0, 255);
		actionChangePheromone(ok1 && ok2, static_cast<uint8_t>(type), id);

	} else if (!strncmp(argv[0], "RECHARGE_PHEROMONE", 19) && argc == 2) {
		bool ok;
		int id = param_int(argv[1], ok, 0, 255);
		actionRechargePheromone(ok, id);

	} else if (!strncmp(argv[0], "ATTACK", 7) && argc == 2) {
		bool ok;
		int id = param_int(argv[1], ok, 0, 255);
		actionAttack(ok, id);

	} else if (!strncmp(argv[0], "EXPLORE", 8) && argc == 1) {
		actionExplore(true);

	} else if (!strncmp(argv[0], "TURN", 5) && argc == 2) {
		bool ok;
		int angle = param_int(argv[1], ok, -180, 180);
		actionTurn(ok, angle);

	} else {
		invalidAction();
	}
}
