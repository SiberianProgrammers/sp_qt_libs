#include <QtGui>
#include <QtQuick>

#include "ImageDxs.h"
#include "Arc.h"
#include "ArcFast.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<dxs::Arc>("DXS", 1, 0, "Arc");
    qmlRegisterType<dxs::ArcFast>("DXS", 1, 0, "ArcFast");

    QQuickView view;
    QSurfaceFormat format = view.format();

    format.setSamples(16);
    view.setFormat(format);
    view.setSource(QUrl("qrc:/main.qml"));
    view.setTitle("VectorGraphics");
    view.setWidth(250);
    view.setHeight(350);

    view.show();

    return app.exec();
}
