#pragma once

#include <QDebug>
#include <QtGui>
#include <QtQuick>

namespace sp {
/**
 * @brief Базовый класс приложения для проектов SP.
 *        Регистрирует компоненты и объекты QML.
 */
class SpApplicationPrototype : public QGuiApplication
{
    public:
        SpApplicationPrototype(int &argc, char **argv
                       , const QString &title
                       , int width = 250, int height = 400);

        int exec(const QUrl &source = QUrl("qrc:/Root.qml"));

        QQuickView *view() { return &_view; }

    protected:
        QQuickView _view;
};
}
