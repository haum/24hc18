#include "pheromone.h"
#include <Qt3DExtras/QConeMesh>

namespace {
struct PheromoneMesh : public Qt3DExtras::QConeMesh {
    PheromoneMesh() {
        setTopRadius(0);
        setBottomRadius(0.01f);
        setLength(0.05f);
        setRings(4);
        setSlices(5);
    }
} pheromoneMesh;
}

Pheromone::Pheromone(Qt3DCore::QEntity * parent, float latitude, float longitude, Qt3DRender::QMaterial * teamMaterial) : Qt3DCore::QEntity(parent) {
    setPosition(latitude, longitude);
    addComponent(&pheromoneMesh);
    addComponent(teamMaterial);
    addComponent(&m_transform);
}

void Pheromone::setVisible(bool on){
    setEnabled(on);
}

void Pheromone::setPosition(float latitude, float longitude) {
    QMatrix4x4 m;
    m.rotate(longitude, 0, 1, 0);
    m.rotate(latitude, 0, 0, 1);
    m.translate(1.025f, 0);
    m.rotate(-90, 0, 0, 1);
    m_transform.setMatrix(m);
}
