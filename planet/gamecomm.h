#ifndef GAMECOMM_H
#define GAMECOMM_H

#include "ant.h"
#include "food.h"
#include "nest.h"
#include "pheromone.h"
#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <Qt3DCore/QEntity>

class GameComm : public QObject {
	Q_OBJECT

	Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
	Q_PROPERTY(int refreshRate READ refreshRate NOTIFY refreshRateChanged)

  public:
	explicit GameComm(QObject *parent = nullptr);

	bool connected() { return m_client != nullptr; }
	int refreshRate() { return m_refreshRate; }

  public slots:
	void setRootEntity(Qt3DCore::QEntity *rootEntity);
	void setRefreshRate(qreal percent);

  signals:
	void connectedChanged();
	void refreshRateChanged();

  private slots:
	void onReadyRead();
	void onDisconnected();
	void onTimeout();

  private:
	Qt3DCore::QEntity *m_rootEntity;
	QTcpServer m_server;
	QTcpSocket *m_client{nullptr};
	int m_refreshRate = 500;

	struct EntitiesList {
		std::vector<GameEntity *>::iterator it;
		std::vector<GameEntity *> entities;
	};
	std::vector<std::array<EntitiesList, 4>> m_teamsData;
	std::vector<uint32_t> m_teams;
};

#endif // GAMECOMM_H
