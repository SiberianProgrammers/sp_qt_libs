#pragma once

#include <QtGui>
#include <QtQuick>

/**
 * @brief Базовый класс приложения для проектов DXS.
 *        Регистрирует компоненты и объекты QML.
 */
class DxsApplication : public QGuiApplication
{
    public:
        DxsApplication(int &argc, char **argv
                       , const QString &title
                       , int width = 250, int height = 400);

        int exec(const QUrl &source = QUrl("qrc:/Root.qml"));

        QQuickView *view() { return &_view; }

    protected:
        QQuickView _view;
};
