#include <QDebug>
#include <QGraphicsScene>
#include "ImageDxs.h"

//------------------------------------------------------------------------------
dxs::ImageDxs::ImageDxs(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , _image(new QImage())
{
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    setAntialiasing(true); // По умолчанию antialiasing включен
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::classBegin()
{
    // Ничего нет
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::componentComplete()
{
    _completed = true;

    if (antialiasing()) {
        setAntialiasing(true);

        // На Reting mac'е Qt неправильно рисует в FBO
        #if defined(Q_OS_OSX)
            setRenderTarget (QQuickPaintedItem::Image);
        #else
            setRenderTarget (QQuickPaintedItem::FramebufferObject);
        #endif
    } else {
        setAntialiasing(false);

        // На Reting mac'е и на iOS с выключенным antialiasing Qt неправильно рисует в FBO
        #if defined(Q_OS_OSX) || defined(Q_OS_IOS)
            setRenderTarget (QQuickPaintedItem::Image);
        #else
            setRenderTarget (QQuickPaintedItem::FramebufferObject);
        #endif
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::paint(QPainter *painter)
{
    if (_status != Ready) {
        return;
    }

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter->setPen(pen);

    QImage *image = _image.data();
    if (_blur) {
        qreal sc = qMax(8.0, 4*width()/_image->width());
        image = new QImage(_image->scaledToWidth(static_cast<int>(_image->width()/sc), Qt::SmoothTransformation));
    }

    switch (_fillMode) {
        case Stretch:
            drawStretch(painter, *image);
            break;

        case PreserveAspectFit:
            drawPreserveAcpectFit(painter, *image);
            break;

        case PreserveAspectCrop:
            drawPreserveAspectCrop(painter, *image);
            break;

        case Pad:
            drawPad(painter, *image);
            break;
    }

    if (_blur) {
        delete image;
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::drawPad(QPainter *painter, const QImage &image)
{
    QBrush brush(image);
    painter->setBrush(brush);

    //TODO Сделать поддержку horizontalAlignment и verticalAlignment
    painter->drawRoundedRect(QRectF(0, 0, width(), height()), _radius, _radius);
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::drawStretch(QPainter *painter, const QImage &image)
{
    qreal wi = static_cast<qreal>(image.width());
    qreal hi = static_cast<qreal>(image.height());
    qreal sw = wi / width();
    qreal sh = hi / height();

    QBrush brush(image);
    brush.setTransform(QTransform().scale(1/sw, 1/sh));
    painter->setBrush(brush);
    painter->drawRoundedRect(QRectF(0, 0, width(), height()), _radius, _radius);
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::drawPreserveAcpectFit(QPainter *painter, const QImage &image)
{
    qreal wi = static_cast<qreal>(image.width());
    qreal hi = static_cast<qreal>(image.height());
    qreal sw = wi / width();
    qreal sh = hi / height();

    if (_radius >= 1) {
        QBrush brush(image);

        if (sw > sh) {
            switch (_horizontalAlignment) {
                case AlignLeft:
                    brush.setTransform(QTransform().scale(1/sw, 1/sw).translate(0, sw*height() - hi));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(0, height() - hi/sw, width(), hi/sw), _radius, _radius);
                    break;
                case AlignHCenter:
                    brush.setTransform(QTransform().scale(1/sw, 1/sw).translate(0, 0.5*(sw*height() - hi)));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(0, 0.5*(height() - hi/sw), width(), hi/sw), _radius, _radius);
                    break;
                case AlignRight:
                    brush.setTransform(QTransform().scale(1/sw, 1/sw));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(0, 0, width(), hi/sw), _radius, _radius);
                    break;
            }
        } else {
            switch (_verticalAlignment) {
                case AlignTop:
                    brush.setTransform(QTransform().scale(1/sh, 1/sh).translate(sh*width() - wi, 0));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(width() - wi/sh, 0, wi/sh, height()), _radius, _radius);
                    break;
                case AlignVCenter:
                    brush.setTransform(QTransform().scale(1/sh, 1/sh).translate(0.5*(sh*width() - wi), 0));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(0.5*(width() - wi/sh), 0, wi/sh, height()), _radius, _radius);
                    break;
                case AlignBottom:
                    brush.setTransform(QTransform().scale(1/sh, 1/sh));
                    painter->setBrush(brush);
                    painter->drawRoundedRect(QRectF(0, 0, wi/sh, height()), _radius, _radius);
                    break;
            }
        }
    } // if (_radius == 0) {
    else {
        if (sw > sh) {
            switch (_verticalAlignment) {
                case AlignTop:
                    painter->drawImage(QRectF(0, 0, width(), hi/sw), image);
                    break;
                case AlignVCenter:
                    painter->drawImage(QRectF(0, 0.5*(height() - hi/sw), width(), hi/sw), image);
                    break;
                case AlignBottom:
                    painter->drawImage(QRectF(0, height() - hi/sw, width(), hi/sw), image);
                    break;
            }
        } else {
            switch (_horizontalAlignment) {
                case AlignLeft:
                    painter->drawImage(QRectF(0, 0, wi/sh, height()), image);
                    break;
                case AlignHCenter:
                    painter->drawImage(QRectF(0.5*(width() - wi/sh), 0, wi/sh, height()), image);
                    break;
                case AlignRight:
                    painter->drawImage(QRectF(width() - wi/sh, 0, wi/sh, height()), image);
                    break;
            }
        }
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::drawPreserveAspectCrop(QPainter *painter, const QImage &image)
{
    qreal w = width();
    qreal h = height();
    qreal wi = static_cast<qreal>(image.width());
    qreal hi = static_cast<qreal>(image.height());
    qreal sw = wi / w;
    qreal sh = hi / h;

    QBrush brush(image);

    // Костыль, исправляющий смещение изображение в Brush по вертикали
    QTransform workaround = QTransform().translate(0, _blur ? -4 : -1);

    if (sw > sh) {
        switch (_horizontalAlignment) {
            case AlignLeft:
                brush.setTransform(workaround.scale(1/sh, 1/sh));
                break;
            case AlignRight:
                brush.setTransform(workaround.scale(1/sh, 1/sh).translate(sh*width() - wi, 0));
                break;
            case AlignHCenter:
                brush.setTransform(workaround.scale(1/sh, 1/sh).translate(0.5*(sh*width() - wi), 0));
                break;
        }

    } else {
        switch (_verticalAlignment) {
            case AlignTop:
                brush.setTransform(workaround.scale(1/sw, 1/sw));
                break;
            case AlignBottom:
                brush.setTransform(workaround.scale(1/sw, 1/sw).translate(0, sw*height() - hi));
                break;
            case AlignVCenter:
                brush.setTransform(workaround.scale(1/sw, 1/sw).translate(0, 0.5*(sw*height() - hi)));
                break;
        }
    }

    painter->setBrush(brush);
    painter->drawRoundedRect(QRectF(0, 0, w, h), _radius, _radius);
}

//------------------------------------------------------------------------------
// Обработка сигнала на загрузки изображения в ImageDxsLoader'е.
//------------------------------------------------------------------------------
void dxs::ImageDxs::onImageDxsLoaded(const QString &/*source*/, QWeakPointer<QImage> image)
{
    if (image != _image) {
        return;
    }

    disconnect (&ImageDxsLoader::instance(), 0, this, 0);

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
// Обработка сигнала об ошибке загрузки изображения в ImageDxsLoader'е.
//------------------------------------------------------------------------------
void dxs::ImageDxs::onImageDxsError(const QString &/*source*/, QWeakPointer<QImage> image, const QString &/*reason*/)
{
    if (image != _image) {
        return;
    }

    //TODO Добавить сообщение об ошибке

    disconnect (&ImageDxsLoader::instance(), 0, this, 0);

    *_image = QImage();
    _status = Error;

    setImplicitHeight(_image->height());

    emit sourceSizeChanged(_image->size());
    if (_completed) {
        update();
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::setSource(const QString &source)
{
    if (_source != source) {
        _source = source;

        if (source.isEmpty()) {
            *_image = QImage();
            _status = Null;
        } else {
            if (!_asynchronous) {
                // Изображение из ресурсов
                ImageDxsLoader::instance().get(_source, _image);

                _status = _image->isNull()
                          ? Error
                          : Ready;
            } else {
                _status = Loading;

                // TODO добавить кеширование
                connect (&ImageDxsLoader::instance(), SIGNAL(loaded(const QString&, WeakImage))
                         , SLOT(onImageDxsLoaded(const QString&, WeakImage))
                         , Qt::UniqueConnection);
                connect (&ImageDxsLoader::instance(), SIGNAL(error(const QString&, WeakImage, const QString&))
                         , SLOT(onImageDxsError(const QString&, WeakImage, const QString&))
                         , Qt::UniqueConnection);

                ImageDxsLoader::instance().loadTo (_source, _image);
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
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::setRadius(qreal radius)
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
void dxs::ImageDxs::setFillMode(dxs::ImageDxs::FillMode fillMode)
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
void dxs::ImageDxs::setBlur(bool blur)
{
    if (_blur != blur) {
        _blur = blur;

        emit blurChanged(_blur);
        if (_completed) {
            update();
        }
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::setAsynchronous(bool asynchronous)
{
    if (_asynchronous != asynchronous) {
        _asynchronous = asynchronous;

        emit asynchronousChanged(_asynchronous);
    }
}

//------------------------------------------------------------------------------
void dxs::ImageDxs::setHorizontalAlignment(dxs::ImageDxs::HorizontalAlignment horizontalAlignment)
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
void dxs::ImageDxs::setVerticalAlignment(dxs::ImageDxs::VerticalAlignment verticalAlignment)
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
QString dxs::ImageDxs::source() const {
    return _source;
}

QSize dxs::ImageDxs::sourceSize() const {
    return _image->size();
}

qreal dxs::ImageDxs::radius() const {
    return _radius;
}

dxs::ImageDxs::FillMode dxs::ImageDxs::fillMode() const {
    return _fillMode;
}

dxs::ImageDxs::Status dxs::ImageDxs::status() const {
    return _status;
}

bool dxs::ImageDxs::blur() const {
    return _blur;
}

bool dxs::ImageDxs::asynchronous() const
{
    return _asynchronous;
}

dxs::ImageDxs::HorizontalAlignment dxs::ImageDxs::horizontalAlignment() const {
    return _horizontalAlignment;
}

dxs::ImageDxs::VerticalAlignment dxs::ImageDxs::verticalAlignment() const {
    return _verticalAlignment;
}
