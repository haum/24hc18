#ifndef GAMECOMM_H
#define GAMECOMM_H

#include <QObject>
#include <QTcpServer>

class GameComm : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    explicit GameComm(QObject *parent = nullptr);

    bool connected() { return m_client != nullptr; }

signals:
    void connectedChanged();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer m_server;
    QTcpSocket * m_client {nullptr};
};

#endif // GAMECOMM_H
