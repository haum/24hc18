#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gamecomm.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    GameComm gcomm;
    engine.rootContext()->setContextProperty("gcomm", &gcomm);
    engine.load(QUrl("main.qml"));
    return app.exec();
}
