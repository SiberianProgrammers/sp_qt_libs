#include "ImageSpNode.h"
#include "SpApplicationPrototype.h"
#include <LogSp.h>

//------------------------------------------------------------------------------
sp::ImageSpNode::ImageSpNode(int vertexAtCorner)
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
sp::ImageSpNode::~ImageSpNode()
{
    if (opaqueMaterial()) {
        static_cast<QSGTextureMaterial *>(material())->texture()->deleteLater();
    }
}

//------------------------------------------------------------------------------
void sp::ImageSpNode::setImage(const QImage &image)
{
    QSGTexture *texture;

    if (material()) {
        texture = qApp->view()->createTextureFromImage(image, QQuickWindow::TextureIsOpaque);
        auto *material = static_cast<QSGTextureMaterial*>(this->material());
        material->texture()->deleteLater();
        material->setTexture(texture);
    } else {
        texture = qApp->view()->createTextureFromImage(image);
        auto *material = new QSGOpaqueTextureMaterial;
        material->setTexture(texture);
        material->setFiltering(QSGTexture::Linear);
        material->setMipmapFiltering(QSGTexture::Linear);

        setMaterial(material);
    }
}

//------------------------------------------------------------------------------
void sp::ImageSpNode::setMipmap(bool mipmap)
{
    _mipmap = mipmap;

    auto *material = static_cast<QSGTextureMaterial*>(this->material());
    if (material) {
        material->setMipmapFiltering(_mipmap ? QSGTexture::Linear : QSGTexture::None);
    }
}
