#pragma once

#include <QtGui>
#include <QtQuick>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<sp::SpApplicationPrototype *>(QCoreApplication::instance()))

namespace sp {
/**
 * @brief Базовый класс приложения для проектов SP.
 *        Регистрирует компоненты и объекты QML.
 */
class SpApplicationPrototype : public QGuiApplication
{
    public:
        SpApplicationPrototype(int &argc, char **argv
                              ,const QString &title
                              ,int width = 230, int height = 400);

        int exec(const QUrl &source = QUrl("qrc:/Root.qml"));

        QQuickView *view() { return &_view; }

        /** @brief Возвращает количество пикселей в миллиметре. */
        double mm() const { return _mm; }

    protected:
        QQuickView _view;
        double _mm;
};
}
