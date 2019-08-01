#include "Shadow.h"
#include "SpApplicationPrototype.h"

sp::Shadow::Shadow(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    setAntialiasing(true); // По умолчанию antialiasing включен
}

//------------------------------------------------------------------------------
void sp::Shadow::classBegin()
{
}

//------------------------------------------------------------------------------
void sp::Shadow::componentComplete()
{
    _complete = true;
}

//------------------------------------------------------------------------------
void sp::Shadow::paint(QPainter *painter)
{
    QLinearGradient gradient;

    gradient.setStart(width(),height());
    gradient.setFinalStop(0,0);

    gradient.setColorAt((qreal)1, QColor("#00000000") );
    gradient.setColorAt((qreal)0, _color );

    painter->setBrush(QBrush(gradient));
    painter->setPen(QPen(Qt::NoPen));

    QRectF rect (QPointF(0,0), QPointF(width(),height()));
    qreal rH = qApp->mm() / height() * 100 * 2;
    qreal rW = qApp->mm() / width() * 100 * 2;
    painter->drawRoundedRect(rect, rW, rH, Qt::RelativeSize);

    // Вариант с рисование неполного Rectangle. Надо подумать.
    //QPainterPath path;
    //qreal r = height(); // радиус скругления
    //path.moveTo(r, 0);
    //path.lineTo(width() - r, 0);
    //path.arcTo(width() - 2*r, -r , 2*r,2*r
    //           , 0
    //           , -90 );
    //path.lineTo(r, r);
    //
    //path.moveTo(r, 0);
    //
    //path.arcTo(0, -r , 2*r,2*r
    //           , 180
    //           , 90 );
    //
    //painter->drawPath(path);
}

void sp::Shadow::setColor(const QColor &color)
{
    if (_color != color) {
        _color = color;
        emit colorChanged(_color);

        if (_complete) {
            update();
        }
    }
}

