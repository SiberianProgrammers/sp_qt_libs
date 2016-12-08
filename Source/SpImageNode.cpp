#include "SpImageNode.h"
#include "SpApplicationPrototype.h"
#include <LogSp.h>


sp::SpImageNode::SpImageNode(int vertexAtCorner)
    : _vertexAtCorner(vertexAtCorner)
    , _segmentCount (4*_vertexAtCorner+3)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), _segmentCount);
    geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
    setGeometry(geometry);
    setFlag(QSGNode::OwnsGeometry);
}

sp::SpImageNode::~SpImageNode()
{
    if (opaqueMaterial()) {
        static_cast<QSGOpaqueTextureMaterial *>(opaqueMaterial())->texture()->deleteLater();
    }
}

void sp::SpImageNode::setImage(const QImage &image)
{
    QSGTexture *texture = qApp->view()->createTextureFromImage(image, QQuickWindow::TextureIsOpaque);
    texture->setFiltering(QSGTexture::Linear);
    texture->setMipmapFiltering(QSGTexture::Linear);
    QSGOpaqueTextureMaterial *material = new QSGOpaqueTextureMaterial;
    material->setTexture(texture);
    material->setFiltering(QSGTexture::Linear);
    material->setMipmapFiltering(QSGTexture::Linear);
    setOpaqueMaterial(material);
    setFlag(QSGNode::OwnsOpaqueMaterial);
}
