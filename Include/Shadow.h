#pragma once

#include <QtQuick>
#include <QColor>
#include <QtMath>

namespace sp {
/**
 * @brief Компонент отрисовки тени
 */
class Shadow : public QQuickPaintedItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY (QColor color READ color  WRITE setColor  NOTIFY colorChanged)

    // TODO добавить Enum какая это тень

    public:
        Shadow(QQuickItem *parent = nullptr);

        virtual void classBegin() override;
        virtual void componentComplete() override;
        virtual void paint(QPainter *painter) override;

        QColor color()  const { return _color; }

        void setColor(const QColor &color);

    signals:
        void colorChanged(QColor color);

    protected:
        bool _complete = false;
        QColor _color = QColor("#15000000");
};
} // namespace sp {
