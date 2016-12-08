#include <LogSp.h>
#include <QThread>
#include <QMetaType>
#include <QPainter>
#include <QBrush>

#include "ImageSpLoader.h"

//int id = qRegisterMetaType<sp::SharedImage>();
int id = qRegisterMetaType<sp::SharedImage>("SharedImage");
int id2 = qRegisterMetaType<sp::WeakImage>("WeakImage");

//------------------------------------------------------------------------------
sp::ImageSpLoader& sp::ImageSpLoader::instance()
{
    static ImageSpLoader instance;
    return instance;
}

//------------------------------------------------------------------------------
sp::ImageSpLoader::ImageSpLoader()
    : QObject (nullptr)
{
    moveToThread(&_thread);
    connect (this, SIGNAL(loadTo(const QString&, SharedImage)), SLOT(get(const QString&, SharedImage)));
    connect (this, SIGNAL(renderImageCrop(SharedImage,qreal,qreal,qreal)), SLOT(onRenderImageCrop(SharedImage,qreal,qreal,qreal)));
    _thread.start();
}

//------------------------------------------------------------------------------
void sp::ImageSpLoader::get(const QString &source, SharedImage image)
{
    if (source.startsWith("qrc:/")) {
        if (image->load(source.mid(3))) {
            emit loaded(source, image);
        } else {
            LOG_ERROR(QString("Ошибка загрузки изображения: %1").arg(source));
            emit error(source, image, QString("Ошибка загрузки изображения: %1").arg(source));
        }
        return;
    }

    *image = QImage();
    emit error(source, image, "Неизвестный способ загрузки изображения.");
}

//------------------------------------------------------------------------------
void sp::ImageSpLoader::onRenderImageCrop(sp::SharedImage sourceImage, qreal w, qreal h, qreal radius)
{
    qreal wi = static_cast<qreal>(sourceImage->width());
    qreal hi = static_cast<qreal>(sourceImage->height());
    qreal sw = wi / w;
    qreal sh = hi / h;

    QBrush brush(*sourceImage);

    // Костыль, исправляющий смещение изображение в Brush по вертикали
    //QTransform workaround = QTransform().translate(0, -1);
    QTransform workaround = QTransform();

    if (sw > sh) {
        //Debug!!! Перебросить параметры выравнивания
        //switch (_horizontalAlignment) {
        //    case AlignLeft:
        //        brush.setTransform(workaround.scale(1/sh, 1/sh));
        //        break;
        //    case AlignRight:
        //        brush.setTransform(workaround.scale(1/sh, 1/sh).translate(sh*width() - wi, 0));
        //        break;
        //    case AlignHCenter:
                brush.setTransform(workaround.scale(1/sh, 1/sh).translate(0.5*(sh*w - wi), 0));
        //        break;
        //}

    } else {
        //switch (_verticalAlignment) {
        //    case AlignTop:
        //        brush.setTransform(workaround.scale(1/sw, 1/sw));
        //        break;
        //    case AlignBottom:
        //        brush.setTransform(workaround.scale(1/sw, 1/sw).translate(0, sw*height() - hi));
        //        break;
        //    case AlignVCenter:
                brush.setTransform(workaround.scale(1/sw, 1/sw).translate(0, 0.5*(sw*h - hi)));
        //        break;
        //}
    }

    QPainter painter;
    QImage renderImage(w, h, QImage::Format_ARGB32_Premultiplied);
    renderImage.fill(Qt::transparent);

    painter.begin(&renderImage);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(brush);
        painter.drawRoundedRect(QRect(0, 0, w, h), radius, radius);
    painter.end();

    emit rendered(sourceImage, renderImage);
}
