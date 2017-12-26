#ifndef NEST_H
#define NEST_H

#include "gameentity.h"
#include <Qt3DRender/QMaterial>

struct Nest : public GameEntity {
	Nest(Qt3DCore::QEntity *parent, float latitude, float longitude,
	     Qt3DRender::QMaterial *teamMaterial);
};

#endif // NEST_H
