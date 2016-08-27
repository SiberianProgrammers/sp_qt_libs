#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include "ImageDxs.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<dxs::ImageDxs>("DXS", 1, 0, "ImageDxs");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
