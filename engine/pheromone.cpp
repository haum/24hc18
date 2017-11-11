#include "pheromone.h"

const GameObjectCategory Pheromone::s_category{"PHEROMONE"};

Pheromone::Pheromone(double latitude, double longitude)
	: GameObject{Pheromone::category(), latitude, longitude, 0} {}
