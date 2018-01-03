#include "food.h"

const GameObjectCategory Food::s_category{"FOOD"};

Food::Food(double latitude, double longitude, float initialValue,
           float chargeRate, int chargeMax, float totalChargeMax,
           bool deadIfEmpty)
    : GameObject{Food::category(), latitude, longitude, 0},
      m_available{initialValue}, m_chargeRate{chargeRate},
      m_chargeMax{chargeMax}, m_totalChargeMax{totalChargeMax},
      m_deadIfEmpty{deadIfEmpty} {}

void Food::periodic() {
	if (m_deadIfEmpty && m_available <= 0)
		return;
	float taken = m_chargeMax - m_available;
	if (taken > m_chargeRate)
		taken = m_chargeRate;
	if (taken > m_totalChargeMax)
		taken = m_totalChargeMax;
	if (taken > 0) {
		m_available += taken;
		m_totalChargeMax -= taken;
	}
}

int Food::collect(int quantity) {
	if (quantity < 0)
		return 0;

	if (m_available >= quantity) {
		m_available -= quantity;
		return quantity;
	}
	quantity = static_cast<int>(m_available);
	m_available = 0;
	return quantity;
}
