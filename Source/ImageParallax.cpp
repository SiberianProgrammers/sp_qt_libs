#include "ImageParallax.h"
#include <QDebug>

sp::ImageParallax::ImageParallax(QQuickItem *parent)
    : QQuickItem(parent)
    , _image(new sp::ImageSp(this))
{
    this->setClip(true);
    connect (this, SIGNAL(heightChanged()), this, SLOT(updateImageSize()));
    connect (this, SIGNAL(widthChanged()), this, SLOT(updateImageSize()));



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
void sp::ImageParallax::setIsDebug(bool isDebug)
{
    _isDebug = isDebug;
}

//--------------------------------------------------------------------------
void sp::ImageParallax::updateImageSize()
{
    qreal h = this->height();
    qreal w = this->width();

    if (h > 0 && w > 0) {
        _image->setY(-0.25*h);
        _image->setX(-0.25*w);

        if (_image->height() != h) {
            _image->setHeight(1.5*h);
        }

        if (_image->width() != w) {
            _image->setWidth(1.5*w);
        }
        _image->setSource(_source);
        _image->componentComplete();
        _imageInit = true;
    }
}

//--------------------------------------------------------------------------
void sp::ImageParallax::updateImagePosition()
{
    if (_isDebug) {
        qDebug() << "_image->width() = " << _image->width() << " _image->height() = " << _image->height()
                 << " _delegate " << (_delegate != nullptr)  << " _relativeItem " << (_relativeItem != nullptr)
                 << "_image->status() = " << _image->status();
    }

    if (_delegate == nullptr || _relativeItem == nullptr
        || _image->width() <= 0 || _image->height() <= 0
        || _image->status() != sp::ImageSp::Ready)
    {
        return;
    }

    if (_orientation == Orientation::Vertical) {
        // Смещаем по оси Y

        // Подогнанный? вариант
        //qreal delta = -1 + 2*(_delegate->y() - _relativeItem->property("contentY").toDouble() - _relativeItem->property("originY").toDouble() + 0.25*_delegate->height()) / (_relativeItem->height() - 0.25*_delegate->height());
        //  qreal delta = (_delegate->y() - _relativeItem->property("contentY").toDouble() - 0.25*_delegate->height()) / (_relativeItem->height());
        //
        //if (_isDebug ) {
        //    qDebug() << "delta = " << delta;
        //}
        //
        //if (delta >= 1) {
        //    delta = 1;
        //} else if (delta <= -1) {
        //    delta = -1;
        //}
        //
        //qreal imageOffset = -0.25*_image->height();
        //_image->setY(imageOffset + delta*imageOffset);

        qreal delta = (_delegate->y() - _relativeItem->property("contentY").toDouble() - _relativeItem->property("originY").toDouble() + _delegate->height()) / (_relativeItem->height());
        delta = delta -1;

        if (delta >= 1) {
            delta = 1;
        } else if (delta <= -1) {
            delta = -1;
        }

        qreal imageOffset = -0.25*_delegate->height();
        _image->setY(imageOffset + delta*imageOffset);

    } else {
        // Смещаем по оси X

        qreal delta = (_delegate->x() - _relativeItem->property("contentX").toDouble() - _relativeItem->property("originX").toDouble() + _delegate->width()) / _relativeItem->width();
        delta = delta - 1;
        if (delta >= 1) {
            delta = 1;
        } else if (delta <= -1) {
            delta = -1;
        }

        qreal imageOffset = -0.25*_delegate->width();

        _image->setX(imageOffset + delta*imageOffset);
    }
}

