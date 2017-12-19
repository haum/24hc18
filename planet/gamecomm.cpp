#include "gamecomm.h"
#include <QColor>
#include <QTcpSocket>
#include <QTimer>
#include <Qt3DExtras/QPhongMaterial>

namespace {
struct TeamMaterial : public Qt3DExtras::QPhongMaterial {
	TeamMaterial(const QColor c) {
		setAmbient(c);
		setShareable(true);
	}
};
TeamMaterial *teamMaterials[] = {
    new TeamMaterial{Qt::yellow}, new TeamMaterial{Qt::cyan},
    new TeamMaterial{Qt::green}, new TeamMaterial{Qt::red},
    new TeamMaterial{Qt::blue}};
constexpr int teamMaterialsSz = sizeof(teamMaterials) / sizeof(*teamMaterials);
} // namespace

GameComm::GameComm(QObject *parent) : QObject(parent) {
	onDisconnected();
	QObject::connect(&m_server, &QTcpServer::newConnection, this, [this] {
		m_client = m_server.nextPendingConnection();
		QObject::connect(m_client, &QTcpSocket::readyRead, this,
		                 &GameComm::onReadyRead);
		QObject::connect(m_client, &QTcpSocket::disconnected, this,
		                 &GameComm::onDisconnected);
		m_server.close();
		emit connectedChanged();
	});
	m_timer.setInterval(500);
	connect(&m_timer, &QTimer::timeout, this, &GameComm::onTimeout);
}

void GameComm::onReadyRead() { qDebug() << "Read" << m_client->readAll(); }

void GameComm::onDisconnected() {
	m_server.listen(QHostAddress::Any, 2080);
	m_client = nullptr;
	emit connectedChanged();
}

void GameComm::onTimeout() {
	// Simple animated test
	if (m_ant) {
		static int p = 0;
		p += 1;
		m_ant->setVisible();
		m_ant->setPosition(p, p, p);
	} else {
		for (int i = 0; i < 8; ++i) {
            new Pheromone(m_rootEntity, 360 / 8 * i, 0, teamMaterials[0]);
            new Pheromone(m_rootEntity, 0, 360 / 8 * i, teamMaterials[0]);
		}
        new Nest(m_rootEntity, 5, 16, teamMaterials[1]);
		new Food(m_rootEntity, 5, 4);
        m_ant = new Ant(m_rootEntity, 5, 8, 90, teamMaterials[2]);
		m_ant->setVisible(false);
	}
}

void GameComm::setRootEntity(Qt3DCore::QEntity *rootEntity) {
	m_rootEntity = rootEntity;
	m_timer.start();
}
