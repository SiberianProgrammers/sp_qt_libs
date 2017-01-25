#include <QtGui>
#include <QtQuick>

#include "DeviceInfo.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
        view.setWidth(app.primaryScreen()->size().width());
        view.setHeight(app.primaryScreen()->size().height());
    #else
        view.setWidth(360);
        view.setHeight(280);
    #endif

    const double mmInInch = 25.4;
    double mm = app.screens().first()->physicalDotsPerInch()/mmInInch;

    view.rootContext()->setContextProperty("mm", mm);
    view.rootContext()->setContextProperty("DeviceInfo", &DeviceInfo::instance());
    view.rootContext()->setContextProperty("Window", &view);

    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));

    #ifdef Q_OS_IOS
        view.showFullScreen();
    #else
        view.show();
    #endif

    return app.exec();
}
