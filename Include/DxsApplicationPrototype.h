#pragma once

#include <QDebug>
#include <QtGui>
#include <QtQuick>

namespace dxs {
/**
 * @brief Базовый класс приложения для проектов DXS.
 *        Регистрирует компоненты и объекты QML.
 */
class DxsApplicationPrototype : public QGuiApplication
{
    public:
        DxsApplicationPrototype(int &argc, char **argv
                       , const QString &title
                       , int width = 250, int height = 400);

        int exec(const QUrl &source = QUrl("qrc:/Root.qml"));

        QQuickView *view() { return &_view; }

    protected:
        QQuickView _view;
};
}
