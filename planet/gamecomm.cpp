#include "gamecomm.h"
#include <QTcpSocket>

GameComm::GameComm(QObject *parent) : QObject(parent) {
    onDisconnected();
    QObject::connect(&m_server, &QTcpServer::newConnection, this, [this] {
        m_client = m_server.nextPendingConnection();
        QObject::connect(m_client, &QTcpSocket::readyRead, this, &GameComm::onReadyRead);
        QObject::connect(m_client, &QTcpSocket::disconnected, this, &GameComm::onDisconnected);
        m_server.close();
        emit connectedChanged();
    });
}

void GameComm::onReadyRead() {
    qDebug() << "Read" << m_client->readAll();
}

void GameComm::onDisconnected() {
    m_server.listen(QHostAddress::Any, 2080);
    m_client = nullptr;
    emit connectedChanged();
}
