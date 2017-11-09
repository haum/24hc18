#include "nest.h"

Nest::Nest(Team &team, double latitude, double longitude,
		   uint64_t initialAntNumber)
	: Agent{team, NEST, latitude, longitude, 0}, m_antNumber{initialAntNumber} {
}

bool Nest::prelude(std::ostream &os) {
	os << "BEGIN NEST\n";
	os << "ANTS " << static_cast<int>(m_antNumber) << '\n';
	os << "END\n";
	os.flush();
	return true;
}

void Nest::execute(uint8_t /*argc*/, const char ** /*argv*/) {}
