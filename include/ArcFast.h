#pragma once

#include <QQuickItem>

namespace sp {
/**
 * @brief Класс для отрисовки дуги через Scene Graph. Для более плавной отрисовки смотри Arc
 * @note Антиалиасинг осуществляется с помощью QSurfaceFormat format; format.setSample(16).
 *       Подробнее смотри в примере в файле main.cpp.
 */
class ArcFast : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (double penWidth     READ penWidth     WRITE setPenWidth     NOTIFY penWidthChanged)
    /** @brief Начальный угол */
    Q_PROPERTY (double startAngle   READ startAngle   WRITE setStartAngle   NOTIFY startAngleChanged)
    /** @brief Угловая длина дуги */
    Q_PROPERTY (double spanAngle    READ spanAngle    WRITE setSpanAngle    NOTIFY spanAngleChanged)
    Q_PROPERTY (QColor color        READ color        WRITE setColor        NOTIFY colorChanged)
    Q_PROPERTY (int    segmentCount READ segmentCount WRITE setSegmentCount NOTIFY segmentCountChanged)

    public:
        ArcFast(QQuickItem *parent = nullptr);

        virtual void classBegin() override;
        virtual void componentComplete() override;
        QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        double penWidth() const;
        double startAngle() const;
        double spanAngle() const;
        QColor color() const;
        int    segmentCount() const;

        void setPenWidth(double penWidth);
        void setStartAngle(double startAngle);
        void setSpanAngle(double spanAngle);
        void setColor(const QColor &color);
        void setSegmentCount(int segmentCount);

    signals:
        void penWidthChanged(double penWidth);
        void startAngleChanged(double startAngle);
        void spanAngleChanged(double spanAngle);
        void colorChanged(QColor color);
        void segmentCountChanged(int segemntCount);

    protected:
        bool _complete = false;
        double _penWidth = 20;
        double _startAngle = 0;
        double _spanAngle = 0;
        QColor _color = Qt::black;
        int _segmentCount = 64;
};
} // namespace sp {
