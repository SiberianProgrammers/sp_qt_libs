#include <QSGSimpleRectNode>
#include <QtMath>
#include "ArcFast.h"

dxs::ArcFast::ArcFast(QQuickItem *parent)
    : QQuickItem (parent)
{
    setFlag(QQuickItem::ItemHasContents);
}

void dxs::ArcFast::classBegin()
{
    // Ничего нет
}

void dxs::ArcFast::componentComplete()
{
    _complete = true;
}

//------------------------------------------------------------------------------
QSGNode* dxs::ArcFast::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), _segmentCount);
        geometry->setLineWidth(_penWidth);
        geometry->setDrawingMode(GL_LINE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(_segmentCount);
    }

    QRectF bounds = boundingRect();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    for (int i = 0; i < _segmentCount; ++i) {
        qreal t = i / qreal(_segmentCount - 1);
        qreal angle = qDegreesToRadians(_startAngle + _spanAngle*t);

        float x = bounds.x() + (1 + qCos(angle)) * bounds.width()/2;
        float y = bounds.y() + (1 - qSin(angle)) * bounds.height()/2;

        vertices[i].set(x, y);
    }
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

//------------------------------------------------------------------------------
double dxs::ArcFast::penWidth() const
{
    return _penWidth;
}

double dxs::ArcFast::startAngle() const
{
    return _startAngle;
}

double dxs::ArcFast::spanAngle() const
{
    return _spanAngle;
}

QColor dxs::ArcFast::color() const
{
    return _color;
}

int dxs::ArcFast::segmentCount() const
{
    return _segmentCount;
}

//------------------------------------------------------------------------------
void dxs::ArcFast::setPenWidth(double penWidth)
{
    if (_penWidth != penWidth) {
        _penWidth = penWidth;
        emit penWidthChanged(_penWidth);

        if (_complete) {
            update();
        }
    }
}

void dxs::ArcFast::setStartAngle(double startAngle)
{
    if (_startAngle != startAngle) {
        _startAngle = startAngle;
        emit startAngleChanged(_startAngle);

        if (_complete) {
            update();
        }
    }
}

void dxs::ArcFast::setSpanAngle(double spanAngle)
{
    if (_spanAngle != spanAngle) {
        _spanAngle = spanAngle;
        emit spanAngleChanged(_spanAngle);

        if (_complete) {
            update();
        }
    }
}

void dxs::ArcFast::setColor(const QColor &color)
{
    if (_color != color) {
        _color = color;
        emit colorChanged(_color);

        if (_complete) {
            update();
        }
    }
}

void dxs::ArcFast::setSegmentCount(int segmentCount)
{
    if (_segmentCount != segmentCount) {
        _segmentCount = segmentCount;
        emit segmentCountChanged(_segmentCount);

        if (_complete) {
            update();
        }
    }
}
