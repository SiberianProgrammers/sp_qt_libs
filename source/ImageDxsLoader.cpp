#include <QDebug>
#include <QThread>
#include "ImageDxsLoader.h"

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
    //_thread.start();
    //moveToThread(&_thread);
    connect (this, SIGNAL(loadTo(const QString&, QImage*)), this, SLOT(get(const QString&, QImage*)));
}

//------------------------------------------------------------------------------
void dxs::ImageDxsLoader::get(const QString &source, QImage *image)
{
    qDebug() << "Загружаем изображение :" << source;

    if (source.startsWith("qrc:/")) {
        image->load(source.mid(3));
        emit loaded(source, image);
        return;
    }

    *image = QImage();
    emit error(source, image, "Неизвестный способ загрузки изображения.");
}
