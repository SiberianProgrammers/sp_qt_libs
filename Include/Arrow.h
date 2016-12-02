#pragma once

#include <QQuickItem>

namespace sp {
/**
 * @brief Класс для отрисовки стрелки через Scene Graph.
 * @note Антиалиасинг осуществляется с помощью QSurfaceFormat format; format.setSample(16).
 *       Подробнее смотри в примере в файле main.cpp.
 */
class Arrow : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (double penWidth     READ penWidth     WRITE setPenWidth     NOTIFY penWidthChanged)
    Q_PROPERTY (QColor color        READ color        WRITE setColor        NOTIFY colorChanged)

    public:
        Arrow(QQuickItem *parent = nullptr);

        virtual void classBegin() override;
        virtual void componentComplete() override;
        QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        double penWidth() const { return _penWidth; }
        QColor color() const { return _color; }

        void setPenWidth(double penWidth);
        void setColor(const QColor &color);

    signals:
        void penWidthChanged(double penWidth);
        void colorChanged(QColor color);

    protected:
        bool _complete = false;
        double _penWidth = 20;
        QColor _color = Qt::black;
        int _vertexCount = 10;
};
} // namespace sp {
