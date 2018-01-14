#include "pheromone.h"
#include "scenario.h"

const GameObjectCategory Pheromone::s_category{"PHEROMONE"};

Pheromone::Pheromone(double latitude, double longitude, Team &team,
                     uint16_t type)
    : GameObject{Pheromone::category(), latitude, longitude, 0}, m_life{100},
      m_team{team}, m_type{type} {
	m_teamBase = &team;
}

void Pheromone::periodic() {
	int n_life = this->life();
	this->setLife(n_life - 1);
	if (n_life <= 0) {
		team().scenario().rmGameObject(this);
	}
}
