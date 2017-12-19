#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QMaterial>

class GameEntity : public Qt3DCore::QEntity {
  public:
	GameEntity(Qt3DCore::QEntity *parent, float latitude, float longitude,
	           float azimut,
	           QMatrix4x4 (*fct)(QMatrix4x4) = [](QMatrix4x4 m) { return m; });

	void setVisible(bool on = true);
	void setPosition(float latitude, float longitude, float azimut);

  protected:
	QMatrix4x4 (*m_initialTransform)(QMatrix4x4);
	Qt3DCore::QTransform m_transform;
};

#endif // GAMEENTITY_H
