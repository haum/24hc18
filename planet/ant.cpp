#include "ant.h"
#include <Qt3DExtras/QConeMesh>

namespace {
struct AntMesh : public Qt3DExtras::QConeMesh {
	AntMesh() {
		setTopRadius(0);
		setBottomRadius(0.005f);
		setLength(0.01f);
		setRings(2);
		setSlices(3);
		setShareable(true);
	}
};
auto *antMesh = new AntMesh;
} // namespace

Ant::Ant(Qt3DCore::QEntity *parent, float latitude, float longitude,
         float azimut, Qt3DRender::QMaterial *teamMaterial)
    : GameEntity(parent, latitude, longitude, azimut, [](QMatrix4x4 m) {
	      m.rotate(-90, 0, 1, 0);
	      return m;
      }) {
	addComponent(antMesh);
	addComponent(teamMaterial);
}
