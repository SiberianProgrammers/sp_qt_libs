#include "Arrow.h"

#include <QSGSimpleRectNode>
#include <QtMath>

sp::Arrow::Arrow(QQuickItem *parent)
    : QQuickItem (parent)
{
    setFlag(QQuickItem::ItemHasContents);
}

void sp::Arrow::classBegin()
{
    // Ничего нет
}

void sp::Arrow::componentComplete()
{
    _complete = true;
}

//------------------------------------------------------------------------------
QSGNode* sp::Arrow::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), _vertexCount);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(_vertexCount);
    }

    QRectF bounds = boundingRect();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

    QPointF a(bounds.x()+bounds.height()/2
             ,bounds.y());
    QPointF b(bounds.x()
             ,bounds.y() + bounds.height()/2);
    QPointF c(bounds.x() + bounds.height()/2
             ,bounds.y() + bounds.height());
    QPointF d(bounds.x() + bounds.width()
             ,bounds.y() + bounds.height()/2);

    qreal p = qSin(M_PI/4);
    qreal q = 0.5+1/p;

    vertices[0].set(b.x(), b.y());
    vertices[1].set(c.x(), c.y());
    vertices[2].set(c.x()+p*_penWidth
                   ,c.y()-p*_penWidth);
    vertices[3].set(b.x()+q*_penWidth
                   ,b.y()+0.5*_penWidth);

    vertices[4].set(d.x()
                   ,d.y()+0.5*_penWidth);
    vertices[5].set(d.x()
                   ,d.y()-0.5*_penWidth);
    vertices[6].set(b.x()+q*_penWidth
                   ,b.y()-0.5*_penWidth);
    vertices[7].set(a.x()+p*_penWidth
                   ,a.y()+p*_penWidth);
    vertices[8].set(a.x(), a.y());
    vertices[9].set(b.x(), b.y());

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

//------------------------------------------------------------------------------
void sp::Arrow::setPenWidth(double penWidth)
{
    if (_penWidth != penWidth) {
        _penWidth = penWidth;
        emit penWidthChanged(_penWidth);

        if (_complete) {
            update();
        }
    }
}

void sp::Arrow::setColor(const QColor &color)
{
    if (_color != color) {
        _color = color;
        emit colorChanged(_color);

        if (_complete) {
            update();
        }
    }
}
