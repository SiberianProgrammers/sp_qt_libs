#include "ImageFast.h"

#include <LogSp.h>
#include <QtMath>
#include <QSGTexture>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleTextureNode>
#include <QImage>
#include <SpApplicationPrototype.h>

//------------------------------------------------------------------------------
sp::ImageFast::ImageFast(QQuickItem *parent)
    : QQuickItem (parent)
    , _node(new SpImageNode)
    , _image(new QImage())
{
    setFlag(QQuickItem::ItemHasContents);

}

sp::ImageFast::~ImageFast()
{
}

//------------------------------------------------------------------------------
void sp::ImageFast::classBegin()
{
    // Ничего нет
}

void sp::ImageFast::componentComplete()
{
    _completed = true;
}

//------------------------------------------------------------------------------
QSGNode* sp::ImageFast::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    if (_status != Ready) {
        return nullptr;
    }

    if (_imageUpdated) {
        _node->setImage(*_image);
        _imageUpdated = false;
        _node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    } else {
        _node->markDirty(QSGNode::DirtyGeometry);
    }

    QSGGeometry::TexturedPoint2D *vertices = _node->geometry()->vertexDataAsTexturedPoint2D();

    const int count = _vertexAtCorner; // Количество точек на закруглённый угол

    Coefficients cf = [this]()->Coefficients{
        switch(this->_fillMode) {
            case Stretch           : return coefficientsStretch();
            case PreserveAspectFit : return coefficientsPreserveAspectFit();
            case PreserveAspectCrop: return coefficientsPreserveAspectCrop();
            case Pad               : return coefficientsPad();
            case Parallax          : return coefficientsRectParallax();
        }
    }();

    const float ox = cf.w/2 + cf.x;
    const float oy = cf.h/2 + cf.y;
    const float lx = cf.w/2 + cf.x;
    const float ly = cf.y;

    const float ax = 0 + cf.x;
    const float ay = 0 + cf.y;
    const float bx = 0 + cf.x;
    const float by = cf.h + cf.y;
    const float cx = cf.w + cf.x;
    const float cy = cf.h + cf.y;
    const float dx = cf.w + cf.x;
    const float dy = 0 + cf.y;

    vertices[0].set(ox, oy, ox*cf.tw+cf.tx, oy*cf.th+cf.ty);
    vertices[1].set(lx, ly, lx*cf.tw+cf.tx, ly*cf.th+cf.ty);

    // Левый верхний угол
    //vertices[1].set(ax, ay, ax/w, ay/h);
    int start = 2;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = ax +_radius*(1 - qFastSin(angle));
        float y = ay +_radius*(1 - qFastCos(angle));
        vertices[start+i].set (x, y, x*cf.tw+cf.tx, y*cf.th+cf.ty);
    }

    // Левый нижний угол
    //vertices[2].set(bx, by, bx/w, by/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = bx +_radius*(1 - qFastCos(angle));
        float y = by +_radius*(-1 + qFastSin(angle));
        vertices[start+i].set (x, y, x*cf.tw+cf.tx, y*cf.th+cf.ty);
    }

    // Правый нижний угол
    //vertices[3].set(cx, cy, cx/w, cy/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = cx +_radius*(-1 + qFastSin(angle));
        float y = cy +_radius*(-1 + qFastCos(angle));
        vertices[start+i].set (x, y, x*cf.tw+cf.tx, y*cf.th+cf.ty);
    }

    // Правый верхний угол
    //vertices[4].set(dx, dy, dx/w, dy/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = dx +_radius*(-1 + qFastCos(angle));
        float y = dy +_radius*(1 - qFastSin(angle));
        vertices[start+i].set (x, y, x*cf.tw+cf.tx, y*cf.th+cf.ty);
    }

    vertices[_segmentCount-1].set(lx, ly, lx*cf.tw+cf.tx, ly*cf.th+cf.ty);

    return _node;
}

//------------------------------------------------------------------------------
void sp::ImageFast::setSource(const QString &source)
{
    if (_source != source) {
        _source = source;

        if (source.isEmpty()) {
            *_image = QImage();
            _status = Null;
        } else {
            if (!_asynchronous) {
                // Изображение из ресурсов
                ImageSpLoader::instance().get(_source, _image);

                _status = _image->isNull()
                          ? Error
                          : Ready;
            } else {
                _status = Loading;
                // TODO добавить кеширование
                connect (&ImageSpLoader::instance(), SIGNAL(loaded(const QString&, WeakImage))
                         , SLOT(onImageSpLoaded(const QString&, WeakImage))
                         , Qt::UniqueConnection);
                connect (&ImageSpLoader::instance(), SIGNAL(error(const QString&, WeakImage, const QString&))
                         , SLOT(onImageSpError(const QString&, WeakImage, const QString&))
                         , Qt::UniqueConnection);

                ImageSpLoader::instance().loadTo (_source, _image);
            }
        }

        emit statusChanged(_status);
        emit sourceChanged(_source);

        setImplicitWidth (_image->width());
        setImplicitHeight(_image->height());

        emit sourceSizeChanged(_image->size());
        if (_completed) {
            update();
        }
    }
} // void sp::ImageFast::setSource(const QString &source)

//------------------------------------------------------------------------------
// 1. Обработка сигнала успешной загрузки изображения в ImageSpLoader'е.
//------------------------------------------------------------------------------
void sp::ImageFast::onImageSpLoaded(const QString &/*source*/, QWeakPointer<QImage> image)
{
    if (image != _image) {
        return;
    }

    _imageUpdated = true;
    disconnect (&ImageSpLoader::instance(), 0, this, 0);

    _status = Ready;
    emit statusChanged(_status);

    setImplicitWidth (_image->width());
    setImplicitHeight(_image->height());

    emit sourceSizeChanged(_image->size());
    if (_completed) {
        update();
    }
}

//------------------------------------------------------------------------------
// 1e. Обработка сигнала об ошибке загрузки изображения в ImageSpLoader'е.
//------------------------------------------------------------------------------
void sp::ImageFast::onImageSpError(const QString &/*source*/, QWeakPointer<QImage> image, const QString &/*reason*/)
{
    if (image != _image) {
        return;
    }

    //TODO Добавить сообщение об ошибке

    disconnect (&ImageSpLoader::instance(), 0, this, 0);

    *_image = QImage();
    _status = Error;

    setImplicitHeight(_image->height());

    emit sourceSizeChanged(_image->size());
    if (_completed) {
        update();
    }
}

//------------------------------------------------------------------------------
void sp::ImageFast::setRadius(float  radius)
{
    if (radius != _radius) {
        _radius = radius;

        emit radiusChanged(_radius);
        if (_completed) {
            update();
        }
    }
}

//------------------------------------------------------------------------------
void sp::ImageFast::setFillMode(sp::ImageFast::FillMode fillMode)
{
    if (_fillMode != fillMode) {
        _fillMode = fillMode;

        emit fillModeChanged(_fillMode);
        if (_completed) {
            update();
        }
    }
}

//------------------------------------------------------------------------------
void sp::ImageFast::setAsynchronous(bool asynchronous)
{
    if (_asynchronous != asynchronous) {
        _asynchronous = asynchronous;

        emit asynchronousChanged(_asynchronous);
    }
}

//------------------------------------------------------------------------------
void sp::ImageFast::setHorizontalAlignment(sp::ImageFast::HorizontalAlignment horizontalAlignment)
{
    if (_horizontalAlignment != horizontalAlignment) {
        _horizontalAlignment = horizontalAlignment;

        emit horizontalAlignmentChanged(_horizontalAlignment);
        if (_completed) {
            update();
        }
    }
}

//------------------------------------------------------------------------------
void sp::ImageFast::setVerticalAlignment(sp::ImageFast::VerticalAlignment verticalAlignment)
{
    if (_verticalAlignment != verticalAlignment) {
        _verticalAlignment = verticalAlignment;

        emit verticalAlignmentChanged(_verticalAlignment);
        if (_completed) {
            update();
        }
    }
}

//==============================================================================
// Расчитывает коэфициенты координат текстуры для fillMode = Pad.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::coefficientsPad() const
{
    LOG_WARN("ImageFast: режим Pad пока не поддерживается.");
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())
           ,0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = Stretch.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::coefficientsStretch() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())
           ,0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = PreserveAspectFit.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::coefficientsPreserveAspectFit() const
{
    Coefficients cf;

    float wi = static_cast<float>(_image->width());
    float hi = static_cast<float>(_image->height());
    float w  = static_cast<float>(width());
    float h  = static_cast<float>(height());
    float cw = w/wi;
    float ch = h/hi;

    if (ch < cw) { // вертикальное расположение
        cf.w = wi*ch;
        cf.h = h;
        cf.y = 0;

        cf.tw = static_cast<float>(1/(wi*ch));
        cf.th = static_cast<float>(1/h);
        cf.ty = 0;

        switch (_horizontalAlignment) {
            case AlignLeft:
                cf.x = 0;
                cf.tx = 0;
                break;
            case AlignHCenter:
                cf.x = (w-cf.w)/2;
                cf.tx = (1-w*cf.tw)/2;
                break;
            case AlignRight:
                cf.x = w-cf.w;
                cf.tx = 1-w*cf.tw;
                break;
        }
    } else { // горизонтальное расположение
        cf.w = w;
        cf.h = hi*cw;
        cf.x = 0;

        cf.tw = static_cast<float>(1/w);
        cf.th = static_cast<float>(1/(hi*cw));
        cf.tx = 0;

        switch (_verticalAlignment) {
            case AlignTop:
                cf.y = 0;
                cf.ty = 0;
                break;
            case AlignVCenter:
                cf.y = (h-cf.h)/2;
                cf.ty = (1-h*cf.th)/2;
                break;
            case AlignBottom:
                cf.y = h-cf.h;
                cf.ty = 1-h*cf.th;
                break;
        }
    }

    return cf ;
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = PreserveAspectCrop.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::coefficientsPreserveAspectCrop() const
{
    Coefficients cf = {0, 0, static_cast<float>(width()), static_cast<float>(height()), 0, 0, 0, 0};

    float wi = static_cast<float>(_image->width());
    float hi = static_cast<float>(_image->height());
    float w  = static_cast<float>(width());
    float h  = static_cast<float>(height());
    float cw = w/wi;
    float ch = h/hi;

    if (ch < cw) {       // вертикальное расположение
        cf.tw = 1/w;
        cf.th = 1/(cw*hi);
        cf.tx = 0;

        switch (_verticalAlignment) {
            case AlignTop:     cf.ty = 0; break;
            case AlignVCenter: cf.ty = (1-h*cf.th)/2; break;
            case AlignBottom:  cf.ty = 1-h*cf.th; break;
        }
    } else {            // горизонтальное расположение
        cf.tw = 1/(ch*wi);
        cf.th = 1/h;
        cf.ty = 0;

        switch (_horizontalAlignment) {
            case AlignLeft:    cf.tx = 0; break;
            case AlignHCenter: cf.tx = (1-w*cf.tw)/2; break;
            case AlignRight:   cf.tx = 1-w*cf.tw; break;
        }
    }

    return cf;
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = Parallax.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::coefficientsRectParallax() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())
           ,0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}
