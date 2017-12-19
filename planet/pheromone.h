#ifndef PHEROMONE_H
#define PHEROMONE_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMaterial>
#include <Qt3DCore/QTransform>

class Pheromone : Qt3DCore::QEntity {
public:
    Pheromone(Qt3DCore::QEntity * parent, float latitude, float longitude, Qt3DRender::QMaterial * teamMaterial);

    void setVisible(bool on = true);
    void setPosition(float latitude, float longitude);

private:
    Qt3DCore::QTransform m_transform;
};

#endif // PHEROMONE_H
