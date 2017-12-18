#include "nest.h"

const GameObjectCategory Nest::s_category{"NEST"};

Nest::Nest(Team &team, double latitude, double longitude,
           uint64_t initialAntNumber)
    : Agent{team, Nest::category(), latitude, longitude, 0},
      m_antNumber{initialAntNumber} {
	m_teamBase = &team;
}

bool Nest::prelude(std::ostream &os) {
	os << "BEGIN NEST\n";
	os << "ANTS " << static_cast<int>(m_antNumber) << '\n';
	os << "END\n";
	os.flush();
	return true;
}

void Nest::execute(uint8_t /*argc*/, const char ** /*argv*/) {}
