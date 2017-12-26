#include "nest.h"
#include "ant.h"
#include "gears/rand.h"
#include "scenario.h"

const GameObjectCategory Nest::s_category{"NEST"};

Nest::Nest(Team &team, double latitude, double longitude)
    : Agent{team, Nest::category(), latitude, longitude, 0} {
	m_teamBase = &team;
	::memset(m_memory, 0, sizeof(m_memory));
}

void Nest::setPosition(double latitude, double longitude) {
	m_latitude = latitude;
	m_longitude = longitude;
}

void Nest::setPopulation(uint8_t type, int nb) {
	if (nb > 0)
		m_antNumber[type] = static_cast<uint32_t>(nb);
	else
		m_antNumber.erase(type);
}

void Nest::setFood(int amount) {
	if (amount > 0)
		m_stock = static_cast<uint32_t>(amount);
	else
		m_stock = 0;
}

bool Nest::prelude(std::ostream &os) {
	os << "BEGIN NEST\n";
	os << "STOCK " << static_cast<int>(m_stock) << '\n';
	os << "MEMORY";
	for (auto m : m_memory)
		os << ' ' << static_cast<int>(m);
	os << '\n';
	for (auto &pair : m_antNumber)
		os << "ANT_COUNT " << static_cast<int>(pair.first) << ' '
		   << static_cast<int>(pair.second) << '\n';
	for (auto &antin : m_antsIn)
		os << "ANT_IN " << static_cast<int>(antin.type) << ' '
		   << static_cast<int>(antin.m0) << ' ' << static_cast<int>(antin.m1)
		   << '\n';
	m_antsIn.clear();
	os << "END\n";
	os.flush();
	return true;
}

void Nest::invalidAction() { log("Invalid action, ignored"); }

void Nest::periodic() {
	uint32_t count = 0;
	for (auto &pair : m_antNumber)
		count += pair.second;
	uint32_t cost = (count / 100) + 1;
	if (m_stock > cost)
		m_stock -= cost;
	else
		m_stock = 0;
}

bool Nest::hasAntType(uint8_t type) { return (m_antNumber.count(type) > 0); }

void Nest::actionAntOut(bool valid, uint8_t type, uint8_t m0, uint8_t m1) {
	if (!valid) {
		invalidAction();
		return;
	}
	if (hasAntType(type) && m_antNumber[type] > 0) {
		team().scenario().addGameObject<Ant>(team(), 200, this->longitude(),
		                                     this->latitude(), random_angle(),
		                                     type, m0, m1);
		setPopulation(type, m_antNumber[type] - 1);
	}
}

void Nest::actionAntNew(bool valid, uint8_t type) {
	if (!valid) {
		invalidAction();
		return;
	}
	if (hasAntType(type)) {
		m_antNumber[type] += 1;
	} else {
		m_antNumber[type] = 1;
	}
}

void Nest::actionMemory(bool valid, uint8_t mem[20]) {
	if (!valid) {
		invalidAction();
		return;
	}
	::memcpy(m_memory, mem, sizeof(m_memory));
}

void Nest::execute(uint8_t argc, const char **argv) {
	if (argc <= 0)
		return;

	if (!strncmp(argv[0], "ANT_OUT", 8) && argc == 4) {
		bool ok_type, ok_m0, ok_m1;
		int type = param_int(argv[1], ok_type, 0, 255);
		int m0 = param_int(argv[2], ok_m0, 0, 255);
		int m1 = param_int(argv[3], ok_m1, 0, 255);
		actionAntOut(ok_type && ok_m0 && ok_m1, static_cast<uint8_t>(type),
		             static_cast<uint8_t>(m0), static_cast<uint8_t>(m1));

	} else if (!strncmp(argv[0], "ANT_NEW", 8) && argc == 2) {
		bool ok;
		int type = param_int(argv[1], ok, 0, 255);
		actionAntNew(ok, static_cast<uint8_t>(type));

	} else if (!strncmp(argv[0], "MEMORY", 7) && argc == 21) {
		bool ok = true;
		uint8_t mem[20];
		for (int i = 1; i < 21; ++i) {
			bool ok_i;
			mem[i - 1] = static_cast<uint8_t>(param_int(argv[i], ok_i, 0, 255));
			ok &= ok_i;
		}
		actionMemory(ok, mem);

	} else {
		invalidAction();
	}
}
