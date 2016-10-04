#include <QDebug>
#include <QThread>
#include <QMetaType>

#include "ImageDxsLoader.h"

//int id = qRegisterMetaType<dxs::SharedImage>();
int id = qRegisterMetaType<dxs::SharedImage>("SharedImage");
int id2 = qRegisterMetaType<dxs::WeakImage>("WeakImage");

//------------------------------------------------------------------------------
dxs::ImageDxsLoader& dxs::ImageDxsLoader::instance()
{
    static ImageDxsLoader instance;
    return instance;
}

//------------------------------------------------------------------------------
dxs::ImageDxsLoader::ImageDxsLoader()
    : QObject (nullptr)
{
    moveToThread(&_thread);
    connect (this, SIGNAL(loadTo(const QString&, SharedImage)), SLOT(get(const QString&, SharedImage)));
    _thread.start();
}

//------------------------------------------------------------------------------
void dxs::ImageDxsLoader::get(const QString &source, QSharedPointer<QImage> image)
{
    if (source.startsWith("qrc:/")) {
        image->load(source.mid(3));
        emit loaded(source, image);
        return;
    }

    *image = QImage();
    emit error(source, image, "Неизвестный способ загрузки изображения.");
}
