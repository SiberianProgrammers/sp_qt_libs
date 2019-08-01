#ifndef ImageSaverSp_H
#define ImageSaverSp_H

#include <QObject>

namespace sp {
class ImageSaverSp: public QObject
{
    Q_OBJECT

    public:
        ImageSaverSp(QObject *parent=0);

        Q_INVOKABLE QString save (const QString &imageUrl, const QString &albumName);
        Q_INVOKABLE bool checkFile(const QString &imageUrl, const QString &albumName);

    signals:
        void successSave();
        void cancelSave();
        void noAccess();

    private:
        QString saveImage(const QString &imageUrl, const QString &albumName);
        void androidUpdateGalleryApp(const QString& filePath);
        QString getImageNameFromURL(const QString& imageURL);

    private:
        const QString pictureLocation;
};
}

#endif // ImageSaverSp_H
