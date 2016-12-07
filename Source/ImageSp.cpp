#include <LogSp.h>
#include <QGraphicsScene>
#include "ImageSp.h"

//------------------------------------------------------------------------------
sp::ImageSp::ImageSp(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , _image(new QPixmap())
{
    setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    setAntialiasing(true); // По умолчанию antialiasing включен

    connect(this, SIGNAL(widthChanged()), SLOT(renderImage()));
    connect(this, SIGNAL(heightChanged()), SLOT(renderImage()));
}

//------------------------------------------------------------------------------
void sp::ImageSp::classBegin()
{
    // Ничего нет
}

//------------------------------------------------------------------------------
void sp::ImageSp::componentComplete()
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

    renderImage();
}

//------------------------------------------------------------------------------
void sp::ImageSp::paint(QPainter *painter)
{
    if (_status != Ready) {
        return;
    }

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter->setPen(pen);

    QPixmap *image = _image.data();
    if (_blur) {
        qreal sc = qMax(8.0, 4*width()/_image->width());
        image = new QPixmap(_image->scaledToWidth(static_cast<int>(_image->width()/sc), Qt::SmoothTransformation));
    }

    switch (_fillMode) {
        case Stretch:
            drawStretch(painter, *image);
            break;

        case PreserveAspectFit:
            drawPreserveAcpectFit(painter, *image);
            break;

        case PreserveAspectCrop:
            //Debug!!!
            //drawPreserveAspectCrop(painter, *image);
            painter->drawPixmap(QPoint(0,0), _renderImage);
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
void sp::ImageSp::drawPad(QPainter *painter, const QPixmap &image)
{
    QBrush brush(image);
    painter->setBrush(brush);

    //TODO Сделать поддержку horizontalAlignment и verticalAlignment
    painter->drawRoundedRect(QRectF(0, 0, width(), height()), _radius, _radius);
}

//------------------------------------------------------------------------------
void sp::ImageSp::drawStretch(QPainter *painter, const QPixmap &image)
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
void sp::ImageSp::drawPreserveAcpectFit(QPainter *painter, const QPixmap &image)
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
                    painter->drawPixmap(QRect(0, 0, width(), hi/sw), image);
                    break;
                case AlignVCenter:
                    painter->drawPixmap(QRect(0, 0.5*(height() - hi/sw), width(), hi/sw), image);
                    break;
                case AlignBottom:
                    painter->drawPixmap(QRect(0, height() - hi/sw, width(), hi/sw), image);
                    break;
            }
        } else {
            switch (_horizontalAlignment) {
                case AlignLeft:
                    painter->drawPixmap(QRect(0, 0, wi/sh, height()), image);
                    break;
                case AlignHCenter:
                    painter->drawPixmap(QRect(0.5*(width() - wi/sh), 0, wi/sh, height()), image);
                    break;
                case AlignRight:
                    painter->drawPixmap(QRect(width() - wi/sh, 0, wi/sh, height()), image);
                    break;
            }
        }
    }
}

//------------------------------------------------------------------------------
void sp::ImageSp::drawPreserveAspectCrop(QPainter *painter, const QPixmap &image)
{
}

//------------------------------------------------------------------------------
// 0. Устанавливает источник изображение и начинает его загрузку.
//------------------------------------------------------------------------------
void sp::ImageSp::setSource(const QString &source)
{
    if (_source != source) {
        _source = source;

        if (source.isEmpty()) {
            *_image = QPixmap();
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
}

//------------------------------------------------------------------------------
// 1. Обработка сигнала успешной загрузки изображения в ImageSpLoader'е.
//------------------------------------------------------------------------------
void sp::ImageSp::onImageSpLoaded(const QString &/*source*/, QWeakPointer<QPixmap> image)
{
    if (image != _image) {
        return;
    }

    disconnect (&ImageSpLoader::instance(), 0, this, 0);

    if (_fillMode == PreserveAspectCrop) {
        renderImage();
    } else {
        _status = Ready;
        emit statusChanged(_status);
    }

    setImplicitWidth (_image->width());
    setImplicitHeight(_image->height());

    emit sourceSizeChanged(_image->size());
    //if (_completed) {
    //    update();
    //}
}

//------------------------------------------------------------------------------
// 1e. Обработка сигнала об ошибке загрузки изображения в ImageSpLoader'е.
//------------------------------------------------------------------------------
void sp::ImageSp::onImageSpError(const QString &/*source*/, QWeakPointer<QPixmap> image, const QString &/*reason*/)
{
    if (image != _image) {
        return;
    }

    //TODO Добавить сообщение об ошибке

    disconnect (&ImageSpLoader::instance(), 0, this, 0);

    *_image = QPixmap();
    _status = Error;

    setImplicitHeight(_image->height());

    emit sourceSizeChanged(_image->size());
    if (_completed) {
        update();
    }
}

//------------------------------------------------------------------------------
// 2. Отрисовка изображения нужных размеров
//------------------------------------------------------------------------------
void sp::ImageSp::renderImage()
{
    if (width() && height() && !_image->isNull() && _completed) {
        connect (&ImageSpLoader::instance(), SIGNAL(rendered(WeakImage,QPixmap))
                 , SLOT(onImageSpRendered(WeakImage,QPixmap))
                 , Qt::UniqueConnection);

        _inRender = true;
        ImageSpLoader::instance().renderImageCrop(_image, width(), height(), _radius);
    }
}

//------------------------------------------------------------------------------
// 3. Отрисовка завершиалась.
//------------------------------------------------------------------------------
void sp::ImageSp::onImageSpRendered(sp::WeakImage sourceImage, QPixmap renderImage)
{
    if (_image != sourceImage) {
        return;
    }

    _renderImage = renderImage;
    _inRender = false;
    _status = Ready;
    emit statusChanged(_status);

    //if ((int)_renderImage.width() != (int)width() || (int)_renderImage.height() != (int)height()) {
    //    ImageSpLoader::instance().renderImageCrop(_image, width(), height(), _radius);
    //}

    if (_completed) {
        update();
    }
}

//------------------------------------------------------------------------------
void sp::ImageSp::setRadius(qreal radius)
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
void sp::ImageSp::setFillMode(sp::ImageSp::FillMode fillMode)
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
void sp::ImageSp::setBlur(bool blur)
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
void sp::ImageSp::setAsynchronous(bool asynchronous)
{
    if (_asynchronous != asynchronous) {
        _asynchronous = asynchronous;

        emit asynchronousChanged(_asynchronous);
    }
}

//------------------------------------------------------------------------------
void sp::ImageSp::setHorizontalAlignment(sp::ImageSp::HorizontalAlignment horizontalAlignment)
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
void sp::ImageSp::setVerticalAlignment(sp::ImageSp::VerticalAlignment verticalAlignment)
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
QString sp::ImageSp::source() const {
    return _source;
}

QSize sp::ImageSp::sourceSize() const {
    return _image->size();
}

qreal sp::ImageSp::radius() const {
    return _radius;
}

sp::ImageSp::FillMode sp::ImageSp::fillMode() const {
    return _fillMode;
}

sp::ImageSp::Status sp::ImageSp::status() const {
    return _status;
}

bool sp::ImageSp::blur() const {
    return _blur;
}

bool sp::ImageSp::asynchronous() const
{
    return _asynchronous;
}

sp::ImageSp::HorizontalAlignment sp::ImageSp::horizontalAlignment() const {
    return _horizontalAlignment;
}

sp::ImageSp::VerticalAlignment sp::ImageSp::verticalAlignment() const {
    return _verticalAlignment;
}
