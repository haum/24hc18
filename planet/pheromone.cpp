#include "pheromone.h"
#include <Qt3DExtras/QConeMesh>

namespace {
struct PheromoneMesh : public Qt3DExtras::QConeMesh {
	PheromoneMesh() {
		setTopRadius(0);
		setBottomRadius(0.001f);
		setLength(0.003f);
		setRings(2);
		setSlices(8);
		setShareable(true);
	}
};
auto *pheromoneMesh = new PheromoneMesh;
} // namespace

Pheromone::Pheromone(Qt3DCore::QEntity *parent, float latitude, float longitude,
                     Qt3DRender::QMaterial *teamMaterial)
    : GameEntity(parent, latitude, longitude, 0, [](QMatrix4x4 m) {
	      m.translate(0.0015f, 0);
	      m.rotate(-90, 0, 0, 1);
	      return m;
      }) {
	addComponent(pheromoneMesh);
	addComponent(teamMaterial);
}
