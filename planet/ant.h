#ifndef ANT_H
#define ANT_H

#include "gameentity.h"
#include <Qt3DRender/QMaterial>

struct Ant : public GameEntity {
	Ant(Qt3DCore::QEntity *parent, float latitude, float longitude,
	    float azimut, Qt3DRender::QMaterial *teamMaterial);
};

#endif // ANT_H
