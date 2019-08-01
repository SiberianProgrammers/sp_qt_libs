#include "Private/ImageSpLoader.h"

#include <LogSp.h>
#include <QThread>
#include <QMetaType>
#include <QPainter>
#include <QBrush>

using namespace sp;

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif
int __idImageSharedPtr = qRegisterMetaType<ImageSharedPtr>("ImageSharedPtr");
int __idImageWeakPtr = qRegisterMetaType<ImageWeakPtr>("ImageWeakPtr");
#ifdef __clang__
#  pragma clang diagnostic pop
#endif

//------------------------------------------------------------------------------
ImageSpLoader::ImageSpLoader()
    : QObject (nullptr)
{
    moveToThread(&_thread);
    connect (this, SIGNAL(loadTo(const QString&, ImageSharedPtr)), SLOT(get(const QString&, ImageSharedPtr)));
    _thread.start();
}

//------------------------------------------------------------------------------
ImageSpLoader& ImageSpLoader::instance()
{
    static ImageSpLoader instance;
    return instance;
}

//------------------------------------------------------------------------------
void ImageSpLoader::get(const QString &source, ImageSharedPtr image)
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

