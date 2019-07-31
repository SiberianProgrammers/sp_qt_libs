#include "ImageParallax.h"

sp::ImageParallax::ImageParallax(QQuickItem *parent)
    : QQuickItem(parent)
    , _image(new sp::ImageSp(this))
{
    this->setClip(true);
    connect (this, SIGNAL(heightChanged()), this, SLOT(updateImageSize()));
    connect (this, SIGNAL(widthChanged()), this, SLOT(updateImageSize()));
    connect (this, SIGNAL(sizeMultiplierChanged(qreal)), this, SLOT(updateImageSize()));

    connect (this, SIGNAL(relativeItemChanged(const QQuickItem*)), this, SLOT(updateImagePosition()));
    connect (this, SIGNAL(delegateChanged(const QQuickItem*)),     this, SLOT(updateImagePosition()));

    connect (_image, SIGNAL(heightChanged()), this, SLOT(updateImagePosition()));
    connect (_image, SIGNAL(widthChanged()),  this, SLOT(updateImagePosition()));
    connect (_image, SIGNAL(statusChanged(const Status)),  this, SLOT(updateImagePosition()));
}

//------------------------------------------------------------------------------
void sp::ImageParallax::classBegin()
{
}

//------------------------------------------------------------------------------
void sp::ImageParallax::componentComplete()
{
    updateImagePosition();
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setOrientation(sp::ImageParallax::Orientation orientation)
{
    if (_orientation != orientation) {
        _orientation = orientation;
        emit orientationChanged(_orientation);
    }
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setRelativeItem(QQuickItem *relativeItem)
{
    _relativeItem = relativeItem;
    emit relativeItemChanged(_relativeItem);
    connect (_relativeItem, SIGNAL(contentYChanged()), this, SLOT(updateImagePosition()));
    connect (_relativeItem, SIGNAL(contentXChanged()), this, SLOT(updateImagePosition()));
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setDelegate(QQuickItem* delegate)
{
    _delegate = delegate;
    emit delegateChanged(_delegate);
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setSource(const QString &source)
{
    _source = source;
    if (_imageInit) {
        _image->setSource(_source);
    }
    emit sourceChanged(_source);
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setAsynchronous(bool asynchronous)
{
    if (_asynchronous != asynchronous) {
        _asynchronous = asynchronous;
        _image->setAsynchronous(asynchronous);
        emit asynchronousChanged(_asynchronous);
    }
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setAntialiasing(bool antialiasing)
{
    if (_image->antialiasing() != antialiasing) {
        _image->setAntialiasing(antialiasing);
        emit antialiasingChanged(antialiasing);
    }
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setFreezed(bool freezed)
{
    _freezed = freezed;
    emit freezedChanged(_freezed);
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setSizeMultiplier(qreal sizeMultiplier)
{
    _sizeMultiplier = sizeMultiplier;
    emit sizeMultiplierChanged(_sizeMultiplier);
}

//--------------------------------------------------------------------------
void sp::ImageParallax::setIsDebug(bool isDebug)
{
    _isDebug = isDebug;
}

//--------------------------------------------------------------------------
void sp::ImageParallax::updateImageSize()
{
    qreal h = this->height();
    qreal w = this->width();

    if (h > 0 && w > 0 && !_freezed) {
        _image->setY(0.5*h*(1 - _sizeMultiplier));
        _image->setX(0.5*w*(1 - _sizeMultiplier));

        if (!qFuzzyCompare(_image->height(), h)) {
            _image->setHeight(_sizeMultiplier*h);
        }

        if (!qFuzzyCompare(_image->width(), w)) {
            _image->setWidth(_sizeMultiplier*w);
        }

        if (!_imageInit) {
            _imageInit = true;
            _image->setSource(_source);
            _image->componentComplete();
        }
    }
}

//--------------------------------------------------------------------------
void sp::ImageParallax::updateImagePosition()
{
    if (_delegate == nullptr || _relativeItem == nullptr
        || _image->width() <= 0
        || _image->height() <= 0
        || _image->status() != sp::ImageSp::Ready
        || _freezed
            )
    {
        return;
    }

    if (_orientation == Orientation::Vertical) {
        // Смещаем по оси Y

        //qreal delta = (_delegate->y() - _relativeItem->property("contentY").toDouble() - _relativeItem->property("originY").toDouble() + _delegate->height()) / (_relativeItem->height());
        //qreal delta = (_delegate->y()/* + _delegate->height()*/ - _relativeItem->property("contentY").toDouble() - _relativeItem->property("originY").toDouble())
        //              / _relativeItem->height();
        //if (delta >= 1) {
        //    delta = 1;
        //} else if (delta <= -1) {
        //    delta = -1;
        //}

        //qreal imageOffset = -0.25*_delegate->height();
        //_image->setY(delta*imageOffset);


        //qreal delta = (_delegate->mapToItem(_relativeItem, QPointF(0,0)).y() + y() + height()/2) / (_relativeItem->height());
        qreal delta = (_delegate->mapToItem(_relativeItem, QPointF(0,0)).y() + y()) / (_relativeItem->height());
        delta = delta -1;

        if (delta >= 1) {
            delta = 1;
        } else if (delta <= -1) {
            delta = -1;
        }



        qreal imageOffset = 0.5*(_image->height() - height());
        _image->setY(delta*imageOffset);
    } else {
        // Смещаем по оси X

        //qreal delta = (_delegate->x() - _relativeItem->property("contentX").toDouble() - _relativeItem->property("originX").toDouble() + _delegate->width()) / _relativeItem->width();
        //delta = delta - 1;
        //if (delta >= 1) {
        //    delta = 1;
        //} else if (delta <= -1) {
        //    delta = -1;
        //}

        //qreal imageOffset = -0.25*_delegate->width();

        //_image->setX(imageOffset + delta*imageOffset);
        qreal delta = (_delegate->mapToItem(_relativeItem, QPointF(0,0)).x() + x() + width()/2) / (_relativeItem->width());
        delta = delta -1;

        if (delta >= 1) {
            delta = 1;
        } else if (delta <= -1) {
            delta = -1;
        }

        qreal imageOffset = _image->width() - width();
        _image->setY(delta*imageOffset);
    }
}

