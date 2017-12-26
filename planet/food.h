#ifndef FOOD_H
#define FOOD_H

#include "gameentity.h"
#include <Qt3DRender/QMaterial>

struct Food : public GameEntity {
	Food(Qt3DCore::QEntity *parent, float latitude, float longitude);
};

#endif // FOOD_H
