#include "ImageSaverSp.h"
#include "Private/ImageSpLoader.h"

#include <QStandardPaths>
#include <QImage>
#include <QPainter>
#include <QDir>

#if defined(Q_OS_ANDROID)
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
    #include <QAndroidActivityResultReceiver>
    #include <QtAndroidExtras>
#endif

using namespace sp;

//----------------------------------------------------------------------------------------------------
ImageSaverSp::ImageSaverSp(QObject *parent) :
    QObject(parent),
    pictureLocation(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation))
{

}

//----------------------------------------------------------------------------------------------------
QString ImageSaverSp::save(const QString &imageUrl, const QString &albumName)
{
    return saveImage(imageUrl, albumName);
}

//----------------------------------------------------------------------------------------------------
bool ImageSaverSp::checkFile(const QString &imageUrl, const QString &albumName)
{
    QString fileName = getImageNameFromURL(imageUrl);
    QString filePath = pictureLocation +"/" + albumName + fileName;
    return  QFile::exists(filePath);
}

//----------------------------------------------------------------------------------------------------
QString ImageSaverSp::saveImage(const QString &imageUrl, const QString &albumName)
{
    QString fileName  = getImageNameFromURL(imageUrl);
    QString folderPath = pictureLocation + "/" + albumName;
    QString filePath = folderPath + "/" + fileName;
    bool success = true;

    // Создаем папку если ее нет
    QDir dir(folderPath);
    if (!dir.exists()){
         success = success && dir.mkdir(folderPath);
    }

    // Получаем картинку, которую надо сохранить
    QSharedPointer<QImage> _image(new QImage());
    ImageSpLoader::instance().get(imageUrl, _image);
    QImage *image = _image.data();

    if (_image.isNull()) {
        return "";
    }

    success = success && image->save (filePath);

    //Возвращаем путь до созданного файла
    QString resultPath = "";
    if (success) {
        resultPath = filePath;
#if defined(Q_OS_ANDROID)
        androidUpdateGalleryApp(filePath);
#endif
    }

    return resultPath;
}

//----------------------------------------------------------------------------------------------------
void ImageSaverSp::androidUpdateGalleryApp(const QString& filePath)
{
#if defined(Q_OS_ANDROID)
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject j_filePath = QAndroidJniObject::fromString(filePath);
    activity.callMethod<void>("refreshGallery", "(Ljava/lang/String;)V"
                                             , j_filePath.object<jstring>());
#endif
}

//----------------------------------------------------------------------------------------------------
QString ImageSaverSp::getImageNameFromURL(const QString &imageURL)
{
    QString imageUrl = imageURL.right(imageURL.length()-imageURL.lastIndexOf("/"));
    imageUrl = imageUrl.replace(QString("?"),"");

    if (imageUrl.right(4).toLower() != "jpeg" && imageUrl.right(3).toLower() != "jpg") {
        imageUrl.append(".jpg");
    }

    return imageUrl;
}
