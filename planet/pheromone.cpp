#include "pheromone.h"
#include <Qt3DExtras/QConeMesh>

namespace {
struct PheromoneMesh : public Qt3DExtras::QConeMesh {
	PheromoneMesh() {
		setTopRadius(0);
		setBottomRadius(0.01f);
		setLength(0.03f);
		setRings(2);
		setSlices(8);
	}
} pheromoneMesh;
} // namespace

Pheromone::Pheromone(Qt3DCore::QEntity *parent, float latitude, float longitude,
                     Qt3DRender::QMaterial *teamMaterial)
    : GameEntity(parent, latitude, longitude, 0, [](QMatrix4x4 m) {
	      m.translate(0.0125f, 0);
	      m.rotate(-90, 0, 0, 1);
	      return m;
      }) {
	addComponent(&pheromoneMesh);
	addComponent(teamMaterial);
}
