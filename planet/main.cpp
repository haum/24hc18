#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQuickView view;

    view.resize(1024, 800);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("main.qml"));
    view.show();

    return app.exec();
}
