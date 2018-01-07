#include "ant.h"
#include "food.h"
#include "gears/utils.h"
#include "pheromone.h"
#include "scenario.h"

static_assert(Ant::WALK_DISTANCE <= Ant::NEAR_DISTANCE,
              "WALK_DISTANCE should be smaller than NEAR_DISTANCE");
static_assert(Ant::NEAR_DISTANCE < Ant::FAR_DISTANCE,
              "NEAR_DISTANCE should be nearer than FAR_DISTANCE");

const GameObjectCategory Ant::s_category{"ANT"};

Ant::Ant(Team &team, int stamina, double latitude, double longitude,
         double heading, uint8_t ant_type, uint8_t memory1, uint8_t memory2)
    : Agent{team, Ant::category(), latitude, longitude, heading},
      m_stamina{std::min(stamina, int(MAX_STAMINA))}, m_stock{0},
      m_attacked{false}, m_ant_type{ant_type}, m_memory{memory1, memory2} {
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
			auto distance = sgo->distance(*this);
			if (distance <= FAR_DISTANCE) {
				size_t gameObjectId = this->team().addId(sgo.get());
				const char *zoneTxt =
				    (distance <= NEAR_DISTANCE ? " NEAR" : " FAR");
				const int distPercentHorizon =
				    static_cast<int>(distance / FAR_DISTANCE * 100);

				if (sgo->category() == Pheromone::category()) {
					auto *pheromone = static_cast<Pheromone *>(sgo.get());
					if (&pheromone->team() == &this->team()) {
						os << "SEE_PHEROMONE";
						os << ' ' << gameObjectId;
						os << zoneTxt;
						os << ' ' << distPercentHorizon;
						os << ' ' << static_cast<int>(pheromone->type());
						os << '\n';
					}

				} else if (sgo->category() == Ant::category()) {
					auto *ant = static_cast<Ant *>(sgo.get());
					bool ownTeam = (&this->team() == &ant->team());
					os << "SEE_ANT";
					os << ' ' << gameObjectId;
					os << zoneTxt;
					os << ' ' << distPercentHorizon;
					os << (ownTeam ? " FRIEND" : " ENEMY");
					os << ' ' << ant->stamina();
					os << '\n';

				} else if (sgo->category() == Nest::category()) {
					auto *nest = static_cast<Nest *>(sgo.get());
					bool ownTeam = (&this->team() == &nest->team());
					os << "SEE_NEST";
					os << ' ' << gameObjectId;
					os << zoneTxt;
					os << ' ' << distPercentHorizon;
					os << (ownTeam ? " FRIEND" : " ENEMY");
					os << '\n';

				} else if (sgo->category() == Food::category()) {
					auto *food = static_cast<Food *>(sgo.get());
					int amount = food->available();
					if (amount > 0) {
						os << "SEE_FOOD";
						os << ' ' << gameObjectId;
						os << zoneTxt;
						os << ' ' << distPercentHorizon;
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
	w() << "Invalid action, kill ant" << std::endl;
	destroy();
}

bool Ant::preludeActionHelper(int cost, ActionType type, bool valid) {
	m_stamina -= cost;
	if (m_stamina < 0) {
		w() << "No more stamina, cannot perform action" << std::endl;
		destroy();
		return false;
	}
	if (type == EXCLUSIVE) {
		if (m_exclusiveDone) {
			w() << "Exclusive action already done, cannot perform action"
			    << std::endl;
			invalidAction();
			return false;
		}
		m_exclusiveDone = true;

	} else if (type == ALWAYS_ALLOWED) {
		// Nothing to do
	}
	if (!valid)
		w() << "Error in command parameters, cannot perform action"
		    << std::endl;
	return valid;
}

GameObject *Ant::getObjectActionHelper(int id, bool &invalid,
                                       GameObject_t cat) {
	if (id <= 0) {
		w() << "Invalid object, cannot perform action" << std::endl;
		invalid = true;
		return nullptr;
	}
	auto index = static_cast<size_t>(id - 1);
	if (index >= team().getIds().size()) {
		w() << "Invalid object, cannot perform action" << std::endl;
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
	if (cat != nullptr && ptr->category() != cat) {
		w() << "Object is not of type " << cat->name()
		    << ", cannot perform action" << std::endl;
		invalid = true;
		return nullptr;
	}
	return ptr;
}

void Ant::actionSetMemory(bool valid, uint8_t m0, uint8_t m1) {
	if (!preludeActionHelper(0, ALWAYS_ALLOWED, valid))
		return;
	m_memory[0] = m0;
	m_memory[1] = m1;
}

void Ant::actionSuicide(bool valid) {
	if (!preludeActionHelper(0, ALWAYS_ALLOWED, valid))
		return;
	destroy();
}

void Ant::actionPutPheromone(bool valid, uint8_t type) {
	if (!preludeActionHelper(3, EXCLUSIVE, valid))
		return;
	team().scenario().addGameObject<Pheromone>(this->latitude(),
	                                           this->longitude(), team(), type);
}

void Ant::actionChangePheromone(bool valid, uint8_t type, int id) {
	if (!preludeActionHelper(2, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Pheromone::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *pheromone =
		    static_cast<Pheromone *>(ptr); // Dynamically checked previously
		pheromone->setType(type);
	} else {
		w() << "Pheromone is no longer present, cannot perform action"
		    << std::endl;
	}
}

void Ant::actionRechargePheromone(bool valid, int id) {
	if (!preludeActionHelper(1, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Pheromone::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *pheromone =
		    static_cast<Pheromone *>(ptr); // Dynamically checked previously
		pheromone->setLife(100);
	} else {
		w() << "Pheromone is no longer present, cannot perform action"
		    << std::endl;
	}
}

void Ant::actionCollect(bool valid, int id, int quantity) {
	if (!preludeActionHelper(4, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Food::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *food = static_cast<Food *>(ptr); // Dynamically checked previously
		int q = std::min(quantity, MAX_STOCK - m_stock);
		q = food->collect(q);
		m_stock += q;
		if (q < quantity)
			w() << "Less food collected than requested" << std::endl;
	} else {
		w() << "Food is no longer present, cannot perform action" << std::endl;
	}
}

void Ant::actionDoTrophallaxis(bool valid, int id, int quantity) {
	int cost = quantity;
	if (!preludeActionHelper(cost, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Ant::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *ant = static_cast<Ant *>(ptr); // Dynamically checked previously
		int q = std::max(0, quantity);
		q = std::min(q, m_stock);
		m_stock -= q;
		q = std::min(q, MAX_STOCK - ant->m_stock);
		ant->m_stock += q;
		if (q < quantity)
			w() << "Less food given than requested" << std::endl;
	} else {
		w() << "Ant is no longer present, cannot perform action" << std::endl;
	}
}

void Ant::actionAttack(bool valid, int id) {
	if (!preludeActionHelper(2, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Ant::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *ant = static_cast<Ant *>(ptr); // Dynamically checked previously
		ant->m_stamina -= 3;
		ant->m_attacked = true;
		if (ant->m_stamina < 0)
			ant->destroy();
	} else {
		w() << "Ant is no longer present, cannot perform action" << std::endl;
	}
}

void Ant::actionEat(bool valid, int quantity) {
	if (!preludeActionHelper(0, EXCLUSIVE, valid))
		return;

	int q = std::max(0, quantity);
	q = std::min(m_stock, q);
	m_stock -= q;
	const auto max_stamina = MAX_STAMINA;
	const auto wanted_stamina = m_stamina + 10 * quantity;
	m_stamina = std::min(m_stamina + 10 * q, max_stamina);
	if (m_stamina < wanted_stamina)
		w() << "Got less stamina than expected" << std::endl;
}

void Ant::actionMoveTo(bool valid, int id) {
	if (!preludeActionHelper(2, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid);
	if (invalid) {
		invalidAction();
		return;
	}

	if (ptr != nullptr) {
		orientToward(*ptr);
		const auto walk_distance = WALK_DISTANCE;
		auto d = std::min(distance(*ptr), walk_distance);
		moveDistance(d);
	} else {
		w() << "Object is no longer present, cannot perform action"
		    << std::endl;
	}
}

void Ant::actionNest(bool valid, int id) {
	if (!preludeActionHelper(2, EXCLUSIVE, valid))
		return;

	bool invalid;
	GameObject *ptr = getObjectActionHelper(id, invalid, Nest::category());
	if (invalid) {
		invalidAction();
		return;
	}

	if ((ptr != nullptr) && (ptr->distance(*this) <= NEAR_DISTANCE)) {
		auto *nest = static_cast<Nest *>(ptr); // Dynamically checked previously
		if (&nest->team() == &team()) {
			nest->antIn(m_ant_type, m_memory[0], m_memory[1],
			            static_cast<unsigned int>(std::max(0, m_stock)));
			destroy();
		} else {
			w() << "The nest is not friend, cannot perform action" << std::endl;
		}
	} else {
		w() << "Nest is no longer present, cannot perform action" << std::endl;
	}
}

void Ant::actionExplore(bool valid) {
	if (!preludeActionHelper(1, EXCLUSIVE, valid))
		return;
	double dh = (random_angle() - M_PI) / 36;
	m_heading = fmod(m_heading + dh, 2 * M_PI);
	moveDistance(WALK_DISTANCE);
}

void Ant::actionTurn(bool valid, int angle) {
	if (!preludeActionHelper(1, EXCLUSIVE, valid))
		return;
	m_heading = fmod(m_heading + angle * M_PI / 180, 2 * M_PI);
}

void Ant::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;
	if ((argc == 3) && (strncmp(argv[0], "SET_MEMORY", 11) == 0)) {
		bool ok1, ok2;
		int m0 = param_int(argv[1], ok1, 0, 255);
		int m1 = param_int(argv[2], ok2, 0, 255);
		actionSetMemory(ok1 && ok2, static_cast<uint8_t>(m0),
		                static_cast<uint8_t>(m1));

	} else if ((argc == 1) && (strncmp(argv[0], "SUICIDE", 8) == 0)) {
		actionSuicide(true);

	} else if ((argc == 2) && (strncmp(argv[0], "PUT_PHEROMONE", 14) == 0)) {
		bool ok = false;
		int type = param_int(argv[1], ok, 0, 255);
		actionPutPheromone(ok, static_cast<uint8_t>(type));

	} else if ((argc == 3) && (strncmp(argv[0], "CHANGE_PHEROMONE", 17) == 0)) {
		bool ok1, ok2;
		int type = param_int(argv[1], ok1, 0, 255);
		int id = param_int(argv[2], ok2, 0, 255);
		actionChangePheromone(ok1 && ok2, static_cast<uint8_t>(type), id);

	} else if ((argc == 2) &&
	           (strncmp(argv[0], "RECHARGE_PHEROMONE", 19) == 0)) {
		bool ok;
		int id = param_int(argv[1], ok, 0, 255);
		actionRechargePheromone(ok, id);

	} else if ((argc == 3) && (strncmp(argv[0], "COLLECT", 8) == 0)) {
		bool ok1, ok2;
		int id = param_int(argv[1], ok1);
		int quantity = param_int(argv[2], ok2);
		actionCollect(ok1 && ok2, id, quantity);

	} else if ((argc == 3) && (strncmp(argv[0], "DO_TROPHALLAXIS", 16) == 0)) {
		bool ok1, ok2;
		int id = param_int(argv[1], ok1);
		int quantity = param_int(argv[2], ok2);
		actionDoTrophallaxis(ok1 && ok2, id, quantity);

	} else if ((argc == 2) && (strncmp(argv[0], "ATTACK", 7) == 0)) {
		bool ok;
		int id = param_int(argv[1], ok, 0, 255);
		actionAttack(ok, id);

	} else if ((argc == 2) && (strncmp(argv[0], "EAT", 4) == 0)) {
		bool ok;
		int quantity = param_int(argv[1], ok);
		actionEat(ok, quantity);

	} else if ((argc == 2) && (strncmp(argv[0], "NEST", 5) == 0)) {
		bool ok;
		int id = param_int(argv[1], ok);
		actionNest(ok, id);

	} else if ((argc == 2) && (strncmp(argv[0], "MOVE_TO", 8) == 0)) {
		bool ok;
		int id = param_int(argv[1], ok);
		actionMoveTo(ok, id);

	} else if ((argc == 1) && (strncmp(argv[0], "EXPLORE", 8) == 0)) {
		actionExplore(true);

	} else if ((argc == 2) && (strncmp(argv[0], "TURN", 5) == 0)) {
		bool ok;
		int angle = param_int(argv[1], ok, -180, 180);
		actionTurn(ok, angle);

	} else {
		invalidAction();
	}
}

void Ant::periodic() {
	m_stamina -= 1;
	if (m_stamina < 0)
		destroy();
}
