#include "nest.h"
#include "ant.h"
#include "gears/rand.h"
#include "scenario.h"

const GameObjectCategory Nest::s_category{"NEST"};

Nest::Nest(Team &team, double latitude, double longitude,
           uint64_t initialAntNumber)
    : Agent{team, Nest::category(), latitude, longitude, 0} {
	addNewAntType(0);
	antQuantityChanger(0, initialAntNumber);
	m_teamBase = &team;
}

bool Nest::prelude(std::ostream &os) {
	os << "BEGIN NEST\n";
	os << "ANTS " << static_cast<int>(getAntNumberOfType(0)) << '\n';
	os << "END\n";
	os.flush();
	return true;
}

void Nest::invalidAction() { log("Invalid action, ignored"); }

void Nest::periodic() {
	unsigned int count = 0;
	for (auto &pair : m_antNumber)
		count += pair.second;
	unsigned int cost = (count / 100) + 1;
	if (m_stock > cost)
		m_stock -= cost;
	else
		m_stock = 0;
}

void Nest::actionAntOut(bool valid, uint8_t type, uint8_t m0, uint8_t m1) {
	if (!valid) {
		invalidAction();
		return;
	}
	team().scenario().addGameObject<Ant>(team(), 200, this->longitude(),
	                                     this->latitude(), random_angle(), type,
	                                     m0, m1);
	antQuantityChanger(type, -1);
}

void Nest::actionAntNew(bool valid, uint8_t type) {
	if (!valid) {
		invalidAction();
		return;
	}
	if (checkAntType(type)) {
		antQuantityChanger(type, 1);
	} else {
		addNewAntType(type);
	}
}

void Nest::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;
	if (!strncmp(argv[0], "ANT_OUT", 8) && argc == 4) {
		bool ok_type;
		int type = param_int(argv[1], ok_type, 0, 255);
		bool ok_m0;
		int m0 = param_int(argv[2], ok_m0, 0, 255);
		bool ok_m1;
		int m1 = param_int(argv[3], ok_m1, 0, 255);
		bool ok = ok_type && ok_m0 && ok_m1;
		actionAntOut(ok, type, m0, m1);
	} else if (!strncmp(argv[0], "ANT_NEW", 8) && argc == 2) {
		bool ok;
		int type = param_int(argv[1], ok, 0, 255);
		actionAntNew(ok, type);
	} else {
		invalidAction();
	}
}
