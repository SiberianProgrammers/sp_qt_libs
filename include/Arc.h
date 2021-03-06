#pragma once

#include <QtQuick>
#include <QColor>

namespace sp {
/**
 * @brief Класс для отрисовки дуги. Использует QPainter. Для быстрой отрисовки смотри ArcFast
 */
class Arc : public QQuickPaintedItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (double penWidth   READ penWidth   WRITE setPenWidth   NOTIFY penWidthChanged)
    Q_PROPERTY (double arcHeight  READ arcHeight   WRITE setArcHeight   NOTIFY arcHeightChanged)
    /** @brief Начальный угол */
    Q_PROPERTY (double startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    /** @brief Угловая длина дуги */
    Q_PROPERTY (double spanAngle  READ spanAngle  WRITE setSpanAngle  NOTIFY spanAngleChanged)
    Q_PROPERTY (QColor color      READ color      WRITE setColor      NOTIFY colorChanged)

    public:
        Arc(QQuickItem *parent = nullptr);

        virtual void classBegin() override;
        virtual void componentComplete() override;
        virtual void paint(QPainter *painter) override;

        double penWidth() const;
        double arcHeight() const;
        double startAngle() const;
        double spanAngle() const;
        QColor color() const;

        void setPenWidth(double penWidth);
        void setArcHeight(double arcHeight);
        void setStartAngle(double startAngle);
        void setSpanAngle(double spanAngle);
        void setColor(const QColor &color);

    signals:
        void penWidthChanged(double penWidth);
        void arcHeightChanged(double arcHeight);
        void startAngleChanged(double startAngle);
        void spanAngleChanged(double spanAngle);
        void colorChanged(QColor color);

    protected:
        bool _complete = false;
        double _penWidth = 2;
        double _startAngle = 0;
        double _spanAngle = 0;
        QColor _color = Qt::black;
        double _arcHeight = 0;
};
} // namespace sp {
