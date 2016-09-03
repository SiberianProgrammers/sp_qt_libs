#include "Arc.h"

dxs::Arc::Arc(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    setAntialiasing(true); // По умолчанию antialiasing включен
}

//------------------------------------------------------------------------------
void dxs::Arc::classBegin()
{
    // Ничего нет
}

//------------------------------------------------------------------------------
void dxs::Arc::componentComplete()
{
    _complete = true;
}

//------------------------------------------------------------------------------
void dxs::Arc::paint(QPainter *painter)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(_penWidth);
    pen.setColor(_color);
    painter->setPen(pen);

    painter->drawArc(QRectF(_penWidth, _penWidth, width()-2*_penWidth, height()-2*_penWidth), static_cast<int>(16*_startAngle), static_cast<int>(16*_spanAngle));
}

//------------------------------------------------------------------------------
double dxs::Arc::penWidth() const
{
    return _penWidth;
}

double dxs::Arc::startAngle() const
{
    return _startAngle;
}

double dxs::Arc::spanAngle() const
{
    return _spanAngle;
}

QColor dxs::Arc::color() const
{
    return _color;
}

//------------------------------------------------------------------------------
void dxs::Arc::setPenWidth(double penWidth)
{
    if (_penWidth != penWidth) {
        _penWidth = penWidth;
        emit penWidthChanged(_penWidth);

        if (_complete) {
            update();
        }
    }
}

void dxs::Arc::setStartAngle(double startAngle)
{
    if (_startAngle != startAngle) {
        _startAngle = startAngle;
        emit startAngleChanged(_startAngle);

        if (_complete) {
            update();
        }
    }
}

void dxs::Arc::setSpanAngle(double spanAngle)
{
    if (_spanAngle != spanAngle) {
        _spanAngle = spanAngle;
        emit spanAngleChanged(_spanAngle);

        if (_complete) {
            update();
        }
    }
}

void dxs::Arc::setColor(const QColor &color)
{
    if (_color != color) {
        _color = color;
        emit colorChanged(_color);

        if (_complete) {
            update();
        }
    }
}
