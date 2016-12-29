#include "Private/ImageSpLoader.h"

#include <LogSp.h>
#include <QThread>
#include <QMetaType>
#include <QPainter>
#include <QBrush>

//int id = qRegisterMetaType<sp::SharedImage>();
int __idImageSharedPtr = qRegisterMetaType<sp::ImageSharedPtr>("ImageSharedPtr");
int __idImageWeakPtr = qRegisterMetaType<sp::ImageWeakPtr>("ImageWeakPtr");

//------------------------------------------------------------------------------
sp::ImageSpLoader::ImageSpLoader()
    : QObject (nullptr)
{
    moveToThread(&_thread);
    connect (this, SIGNAL(loadTo(const QString&, ImageSharedPtr)), SLOT(get(const QString&, ImageSharedPtr)));
    _thread.start();
}

//------------------------------------------------------------------------------
sp::ImageSpLoader& sp::ImageSpLoader::instance()
{
    static ImageSpLoader instance;
    return instance;
}

//------------------------------------------------------------------------------
void sp::ImageSpLoader::get(const QString &source, ImageSharedPtr image)
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

