#include "Arc.h"

sp::Arc::Arc(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    setAntialiasing(true); // По умолчанию antialiasing включен
}

//------------------------------------------------------------------------------
void sp::Arc::classBegin()
{
    // Ничего нет
}

//------------------------------------------------------------------------------
void sp::Arc::componentComplete()
{
    _complete = true;
}

//------------------------------------------------------------------------------
void sp::Arc::paint(QPainter *painter)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(_penWidth);
    pen.setColor(_color);
    painter->setPen(pen);

    if (_arcHeight <= 2*_penWidth) {

         painter->drawLine(QLineF (_penWidth
                                 , 0.5*height()
                                 , width() - _penWidth
                                 , 0.5*height() ));
    } else {
        painter->drawArc(QRectF(_penWidth
                            , _penWidth + 0.5*(height() - _arcHeight)
                            , width()-2*_penWidth
                            , _arcHeight-2*_penWidth
                            )
                            , static_cast<int>(16*_startAngle)
                            , static_cast<int>(16*_spanAngle));
    }
}

//------------------------------------------------------------------------------
double sp::Arc::penWidth() const
{
    return _penWidth;
}

double sp::Arc::arcHeight() const
{
    return _arcHeight;
}

double sp::Arc::startAngle() const
{
    return _startAngle;
}

double sp::Arc::spanAngle() const
{
    return _spanAngle;
}

QColor sp::Arc::color() const
{
    return _color;
}

//------------------------------------------------------------------------------
void sp::Arc::setPenWidth(double penWidth)
{
    if (_penWidth != penWidth) {
        _penWidth = penWidth;
        emit penWidthChanged(_penWidth);

        if (_complete) {
            update();
        }
    }
}

void sp::Arc::setArcHeight(double arcHeight)
{
    if (_arcHeight != arcHeight) {
        _arcHeight = arcHeight;
        emit arcHeightChanged(_arcHeight);

        if (_complete) {
            update();
        }
    }
}

void sp::Arc::setStartAngle(double startAngle)
{
    if (_startAngle != startAngle) {
        _startAngle = startAngle;
        emit startAngleChanged(_startAngle);

        if (_complete) {
            update();
        }
    }
}

void sp::Arc::setSpanAngle(double spanAngle)
{
    if (_spanAngle != spanAngle) {
        _spanAngle = spanAngle;
        emit spanAngleChanged(_spanAngle);

        if (_complete) {
            update();
        }
    }
}

void sp::Arc::setColor(const QColor &color)
{
    if (_color != color) {
        _color = color;
        emit colorChanged(_color);

        if (_complete) {
            update();
        }
    }
}
