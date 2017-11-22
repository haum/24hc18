#include "pheromone.h"

const GameObjectCategory Pheromone::s_category{"PHEROMONE"};

Pheromone::Pheromone(double latitude, double longitude, Team& team, int type)
	: GameObject{Pheromone::category(), latitude, longitude, 0}, m_team{team}, m_type{type} {}
