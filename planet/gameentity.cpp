#include "gameentity.h"
#include <Qt3DExtras/QConeMesh>

GameEntity::GameEntity(Qt3DCore::QEntity *parent, float latitude,
                       float longitude, float azimut,
                       QMatrix4x4 (*fct)(QMatrix4x4))
    : Qt3DCore::QEntity(parent), m_initialTransform(fct) {
	setPosition(latitude, longitude, azimut);
	m_transform.setShareable(true);
	addComponent(&m_transform);
}

void GameEntity::setVisible(bool on) { setEnabled(on); }

void GameEntity::setPosition(float latitude, float longitude, float azimut) {
	QMatrix4x4 m;
	m.rotate(longitude, 0, 1, 0);
	m.rotate(latitude, 0, 0, 1);
	m.translate(1, 0);
	m.rotate(azimut, 1, 0, 0);
	m_transform.setMatrix(m_initialTransform(m));
}
