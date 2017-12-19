#include "gamecomm.h"
#include <QTcpSocket>
#include <QTimer>
#include <QColor>
#include <Qt3DExtras/QPhongMaterial>

namespace {
struct TeamMaterial : public Qt3DExtras::QPhongMaterial {
    TeamMaterial(const QColor c) {
        setAmbient(c);
    }
} teamMaterials[] = {
    {Qt::yellow}, {Qt::cyan}, {Qt::green}, {Qt::red}, {Qt::blue}, {Qt::magenta}
};
}

GameComm::GameComm(QObject *parent) : QObject(parent) {
    onDisconnected();
    QObject::connect(&m_server, &QTcpServer::newConnection, this, [this] {
        m_client = m_server.nextPendingConnection();
        QObject::connect(m_client, &QTcpSocket::readyRead, this, &GameComm::onReadyRead);
        QObject::connect(m_client, &QTcpSocket::disconnected, this, &GameComm::onDisconnected);
        m_server.close();
        emit connectedChanged();
    });
    m_timer.setInterval(500);
    connect(&m_timer, &QTimer::timeout, this, &GameComm::onTimeout);
}

void GameComm::onReadyRead() {
    qDebug() << "Read" << m_client->readAll();
}

void GameComm::onDisconnected() {
    m_server.listen(QHostAddress::Any, 2080);
    m_client = nullptr;
    emit connectedChanged();
}

void GameComm::onTimeout() {
    // Simple animated test
    static int p = 0;
    if (m_pheromones.size() > 0) {
        p += 1;
        auto * ph = *m_pheromones.rbegin();
        ph->setVisible();
        ph->setPosition(p, p);
    } else {
        for (int i = 0; i < 8; ++i) {
            m_pheromones.push_back(new Pheromone(m_rootEntity, 360/8*i, 0, &teamMaterials[0]));
            m_pheromones.push_back(new Pheromone(m_rootEntity, 0, 360/8*i, &teamMaterials[1]));
        }

        auto * ph = new Pheromone(m_rootEntity, 45, 45, &teamMaterials[2]);
        ph->setVisible(false);
        m_pheromones.push_back(ph);
    }
}

void GameComm::setRootEntity(Qt3DCore::QEntity *rootEntity) {
    m_rootEntity = rootEntity;
    m_timer.start();
}
