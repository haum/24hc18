#ifndef GAMECOMM_H
#define GAMECOMM_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <Qt3DCore/QEntity>
#include "pheromone.h"

class GameComm : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    explicit GameComm(QObject *parent = nullptr);

    bool connected() { return m_client != nullptr; }

public slots:
    void setRootEntity(Qt3DCore::QEntity *rootEntity);

signals:
    void connectedChanged();

private slots:
    void onReadyRead();
    void onDisconnected();
    void onTimeout();

private:
    Qt3DCore::QEntity *m_rootEntity;
    std::vector<Pheromone*> m_pheromones;
    QTcpServer m_server;
    QTcpSocket * m_client {nullptr};
    QTimer m_timer;
};

#endif // GAMECOMM_H
