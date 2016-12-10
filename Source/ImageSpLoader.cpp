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

