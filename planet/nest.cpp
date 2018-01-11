#include "nest.h"
#include <Qt3DExtras/QConeMesh>

namespace {
struct NestMesh : public Qt3DExtras::QConeMesh {
	NestMesh() {
		setTopRadius(0);
		setBottomRadius(0.004f);
		setLength(0.006f);
		setRings(4);
		setSlices(5);
		setShareable(true);
	}
};
NestMesh *nestMesh = new NestMesh;
} // namespace

Nest::Nest(Qt3DCore::QEntity *parent, float latitude, float longitude,
           Qt3DRender::QMaterial *teamMaterial)
    : GameEntity(parent, latitude, longitude, 0, [](QMatrix4x4 m) {
	      m.translate(0.003f, 0);
	      m.rotate(-90, 0, 0, 1);
	      return m;
      }) {
	addComponent(nestMesh);
	addComponent(teamMaterial);
}
