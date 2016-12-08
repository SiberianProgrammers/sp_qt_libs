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
    //if (_node.opaqueMaterial()) {
    //    LOG_ALEUS("delete texture");
    //    static_cast<QSGOpaqueTextureMaterial *>(_node.opaqueMaterial())->texture()->deleteLater();
    //}
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

    SpImageNode *node = nullptr;
    QSGGeometry *geometry = nullptr;

    if (!oldNode) {
        node = _node;
        node->setImage(*_image);
        node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    } else {
        node = static_cast<SpImageNode *>(oldNode);
        node->markDirty(QSGNode::DirtyGeometry);
    }

    geometry = node->geometry();
    QSGGeometry::TexturedPoint2D *vertices = geometry->vertexDataAsTexturedPoint2D();

    const int count = _vertexAtCorner; // Количество точек на закруглённый угол

    Coefficients gc = [this]()->Coefficients{
        switch(this->_fillMode) {
            case Stretch           : return gcStretch();
            case PreserveAspectFit : return gcPreserveAspectFit();
            case PreserveAspectCrop: return gcPreserveAspectCrop();
            case Pad               : return gcPad();
            case Parallax          : return gcRectParallax();
        }
    }();

    const float ox = gc.w/2 + gc.x;
    const float oy = gc.h/2 + gc.y;
    const float lx = gc.w/2 + gc.x;
    const float ly = gc.y;

    const float ax = 0 + gc.x;
    const float ay = 0 + gc.y;
    const float bx = 0 + gc.x;
    const float by = gc.h + gc.y;
    const float cx = gc.w + gc.x;
    const float cy = gc.h + gc.y;
    const float dx = gc.w + gc.x;
    const float dy = 0 + gc.y;

    Coefficients tc = [this]()->Coefficients {
        switch (this->_fillMode) {
            case Stretch           : return tcStretch();
            case PreserveAspectFit : return tcPreserveAspectFit();
            case PreserveAspectCrop: return tcPreserveAspectCrop();
            case Pad               : return tcPad();
            case Parallax          : return tcRectParallax();
        }
    }();

    vertices[0].set(ox, oy, ox*tc.w+tc.x, oy*tc.h+tc.y);
    vertices[1].set(lx, ly, lx*tc.w+tc.x, ly*tc.h+tc.y);

    // Левый верхний угол
    //vertices[1].set(ax, ay, ax/w, ay/h);
    int start = 2;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = ax +_radius*(1 - qFastSin(angle));
        float y = ay +_radius*(1 - qFastCos(angle));
        vertices[start+i].set (x, y, x*tc.w+tc.x, y*tc.h+tc.y);
    }

    // Левый нижний угол
    //vertices[2].set(bx, by, bx/w, by/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = bx +_radius*(1 - qFastCos(angle));
        float y = by +_radius*(-1 + qFastSin(angle));
        vertices[start+i].set (x, y, x*tc.w+tc.x, y*tc.h+tc.y);
    }

    // Правый нижний угол
    //vertices[3].set(cx, cy, cx/w, cy/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = cx +_radius*(-1 + qFastSin(angle));
        float y = cy +_radius*(-1 + qFastCos(angle));
        vertices[start+i].set (x, y, x*tc.w+tc.x, y*tc.h+tc.y);
    }

    // Правый верхний угол
    //vertices[4].set(dx, dy, dx/w, dy/h);
    start = start + count;
    for (int i=0; i < count; ++i) {
        float angle = M_PI_2 * static_cast<float>(i) / static_cast<float>(count-1);
        float x = dx +_radius*(-1 + qFastCos(angle));
        float y = dy +_radius*(1 - qFastSin(angle));
        vertices[start+i].set (x, y, x*tc.w+tc.x, y*tc.h+tc.y);
    }

    vertices[_segmentCount-1].set(lx, ly, lx*tc.w+tc.x, ly*tc.h+tc.y);

    return node;
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

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат геометрии для fillMode = Pad.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::gcPad() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат геометрии для fillMode = Stretch.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::gcStretch() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат геометрии для fillMode = PreserveAspectFit.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::gcPreserveAspectFit() const
{
    Coefficients gc = {0, 0, static_cast<float>(width()), static_cast<float>(height())};

    float wi = static_cast<float>(_image->width());
    float hi = static_cast<float>(_image->height());
    float w  = static_cast<float>(width());
    float h  = static_cast<float>(height());
    float cw = w/wi;
    float ch = h/hi;

    if (ch < cw) {       // вертикальное расположение
        gc.w = wi*ch;
        gc.h = h;

        gc.y = 0;
        switch (_horizontalAlignment) {
            case AlignLeft:    gc.x = 0; break;
            case AlignHCenter: gc.x = (w-gc.w)/2; break;
            case AlignRight:   gc.x = w-gc.w; break;
        }
    } else {            // горизонтальное расположение
        gc.w = w;
        gc.h = hi*cw;

        gc.x = 0;
        switch (_verticalAlignment) {
            case AlignTop:     gc.y = 0; break;
            case AlignVCenter: gc.y = (h-gc.h)/2; break;
            case AlignBottom:  gc.y = h-gc.h; break;
        }
    }

    return gc ;
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат геометрии для fillMode = PreserveAspectCrop.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::gcPreserveAspectCrop() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат геометрии для fillMode = Parallax.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::gcRectParallax() const
{
    return {0, 0, static_cast<float>(width()), static_cast<float>(height())};
}

//==============================================================================
// Расчитывает коэфициенты координат текстуры для fillMode = Pad.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::tcPad() const
{
    return {0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = Stretch.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::tcStretch() const
{
    return {0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = PreserveAspectFit.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::tcPreserveAspectFit() const
{
    Coefficients tc = {0, 0, 0, 0};

    float wi = static_cast<float>(_image->width());
    float hi = static_cast<float>(_image->height());
    float w  = static_cast<float>(width());
    float h  = static_cast<float>(height());
    float cw = w/wi;
    float ch = h/hi;

    if (ch < cw) {       // вертикальное расположение
        tc.w = static_cast<float>(1/(wi*ch));
        tc.h = static_cast<float>(1/h);

        tc.y = 0;
        switch (_horizontalAlignment) {
            case AlignLeft:    tc.x = 0; break;
            case AlignHCenter: tc.x = (1-w*tc.w)/2; break;
            case AlignRight:   tc.x = 1-w*tc.w; break;
        }
    } else {            // горизонтальное расположение
        tc.w = static_cast<float>(1/w);
        tc.h = static_cast<float>(1/(hi*cw));

        tc.x = 0;
        switch (_verticalAlignment) {
            case AlignTop:     tc.y = 0; break;
            case AlignVCenter: tc.y = (1-h*tc.h)/2; break;
            case AlignBottom:  tc.y = 1-h*tc.h; break;
        }
    }

    return tc ;
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = PreserveAspectCrop.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::tcPreserveAspectCrop() const
{
    Coefficients tc;

    float wi = static_cast<float>(_image->width());
    float hi = static_cast<float>(_image->height());
    float w  = static_cast<float>(width());
    float h  = static_cast<float>(height());
    float cw = w/wi;
    float ch = h/hi;

    if (ch < cw) {       // вертикальное расположение
        tc.w = 1/w;
        tc.h = 1/(cw*hi);

        tc.x = 0;
        switch (_verticalAlignment) {
            case AlignTop:     tc.y = 0; break;
            case AlignVCenter: tc.y = (1-h*tc.h)/2; break;
            case AlignBottom:  tc.y = 1-h*tc.h; break;
        }
    } else {            // горизонтальное расположение
        tc.w = 1/(ch*wi);
        tc.h = 1/h;

        tc.y = 0;
        switch (_horizontalAlignment) {
            case AlignLeft:    tc.x = 0; break;
            case AlignHCenter: tc.x = (1-w*tc.w)/2; break;
            case AlignRight:   tc.x = 1-w*tc.w; break;
        }
    }

    return tc;
}

//------------------------------------------------------------------------------
// Расчитывает коэфициенты координат текстуры для fillMode = Parallax.
//------------------------------------------------------------------------------
sp::ImageFast::Coefficients sp::ImageFast::tcRectParallax() const
{
    return {0, 0, static_cast<float>(1/width()), static_cast<float>(1/height())};
}
