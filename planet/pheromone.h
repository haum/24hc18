#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "gameentity.h"
#include <Qt3DRender/QMaterial>

struct Pheromone : public GameEntity {
	Pheromone(Qt3DCore::QEntity *parent, float latitude, float longitude,
	          Qt3DRender::QMaterial *teamMaterial);
};

#endif // PHEROMONE_H
