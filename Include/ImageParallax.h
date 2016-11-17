#pragma once

#include <QtQuick>
#include <QImage>
#include <QString>
#include <ImageSp.h>

namespace sp {

class ImageParallax: public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(bool isDebug READ isDebug WRITE setIsDebug)
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QQuickItem* relativeItem  WRITE setRelativeItem NOTIFY relativeItemChanged)
    Q_PROPERTY(QQuickItem* delegate      WRITE setDelegate     NOTIFY delegateChanged)

    // Поля для ImageSp
    Q_PROPERTY (QString  source       READ source       WRITE setSource       NOTIFY sourceChanged)
    Q_PROPERTY (QSize    sourceSize   READ sourceSize                         NOTIFY sourceSizeChanged)
    Q_PROPERTY (bool     asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)
    Q_PROPERTY (bool     antialiasing READ antialiasing WRITE setAntialiasing )
    Q_PROPERTY (sp::ImageSp::Status status READ status NOTIFY statusChanged)

    public:
        enum Orientation {
            Horizontal = 0
            , Vertical = 1
        };

        Q_ENUM (Orientation)

    public:
        ImageParallax(QQuickItem *parent = nullptr);
        virtual void classBegin() override;
        virtual void componentComplete() override;

    public:
        void setOrientation(Orientation orientation);
        void setIsDebug(bool isDebug);
        void setRelativeItem(QQuickItem* relativeItem);
        void setDelegate(QQuickItem* delegate);
        void setSource(const QString &source);
        void setAsynchronous (bool asynchronous);
        void setAntialiasing (bool antialiasing);

        bool isDebug()         const { return _isDebug; }
        QString source()       const { return _source; }
        QSize   sourceSize()   const { return _image->sourceSize(); }
        bool    asynchronous() const { return _asynchronous; }
        Orientation orientation() const { return _orientation; }
        sp::ImageSp::Status status() const { return _image->status(); }

    private slots:
        void updateImageSize();
        void updateImagePosition();

    signals:
        void orientationChanged(const Orientation orientation);
        void delegateChanged(const QQuickItem* delegate);
        void relativeItemChanged(const QQuickItem* relativeItem);

        void sourceChanged(const QString&);
        void sourceSizeChanged(const QSize&);
        void statusChanged(sp::ImageSp::Status);
        void asynchronousChanged(bool);

    private:
        Orientation _orientation = Orientation::Vertical;
        ImageSp *_image;
        QQuickItem* _delegate = nullptr;
        QQuickItem* _relativeItem = nullptr;
        bool _isDebug = false;

        QString  _source;
        bool _blur = false;
        bool _asynchronous = true;
        bool _imageInit = false;
        sp::ImageSp::Status _status = sp::ImageSp::Null;
};
}
