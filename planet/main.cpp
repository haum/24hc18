#include <Qt3DQuickExtras/qt3dquickwindow.h>
#include <QGuiApplication>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    Qt3DExtras::Quick::Qt3DQuickWindow view;
    view.setSource(QUrl("main.qml"));
    view.show();

    return app.exec();
}
