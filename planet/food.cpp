#include "food.h"
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QPhongMaterial>

namespace {
struct FoodMesh : public Qt3DExtras::QCylinderMesh {
	FoodMesh() {
		setRadius(0.01f);
		setLength(0.02f);
		setSlices(32);
		setShareable(true);
	}
};
auto *foodMesh = new FoodMesh;
struct FoodMaterial : public Qt3DExtras::QPhongMaterial {
	FoodMaterial() { setAmbient(Qt::white); }
};
auto *foodMaterial = new FoodMaterial;
} // namespace

Food::Food(Qt3DCore::QEntity *parent, float latitude, float longitude)
    : GameEntity(parent, latitude, longitude, 0) {
	addComponent(foodMesh);
	addComponent(foodMaterial);
}
