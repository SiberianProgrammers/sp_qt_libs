#include "SpImageNode.h"
#include "SpApplicationPrototype.h"
#include <LogSp.h>

//------------------------------------------------------------------------------
sp::SpImageNode::SpImageNode(int vertexAtCorner)
    : _vertexAtCorner(vertexAtCorner)
    , _segmentCount (4*_vertexAtCorner+3)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), _segmentCount);
    geometry->setDrawingMode(QSGGeometry::DrawTriangleFan);
    setGeometry(geometry);

    setFlag(QSGNode::OwnsGeometry);
    setFlag(QSGNode::OwnsOpaqueMaterial);
}

//------------------------------------------------------------------------------
sp::SpImageNode::~SpImageNode()
{
    if (opaqueMaterial()) {
        static_cast<QSGOpaqueTextureMaterial *>(opaqueMaterial())->texture()->deleteLater();
    }
}

//------------------------------------------------------------------------------
void sp::SpImageNode::setImage(const QImage &image)
{
    QSGTexture *texture;

    if (material()) {
        texture = qApp->view()->createTextureFromImage(image, QQuickWindow::TextureIsOpaque);
        auto *material = static_cast<QSGOpaqueTextureMaterial*>(opaqueMaterial());
        material->texture()->deleteLater();
        material->setTexture(texture);
    } else {
        texture = qApp->view()->createTextureFromImage(image, QQuickWindow::TextureIsOpaque);
        auto *material = new QSGOpaqueTextureMaterial;
        material->setTexture(texture);
        material->setFiltering(QSGTexture::Linear);
        material->setMipmapFiltering(QSGTexture::Linear);

        setOpaqueMaterial(material);
    }
}

//------------------------------------------------------------------------------
void sp::SpImageNode::setMipmap(bool mipmap)
{
    _mipmap = mipmap;

    auto *material = static_cast<QSGOpaqueTextureMaterial*>(opaqueMaterial());
    if (material) {
        material->setMipmapFiltering(_mipmap ? QSGTexture::Linear : QSGTexture::None);
    }
}
