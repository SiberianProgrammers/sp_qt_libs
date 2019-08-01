#pragma once

#include "Private/ImageSpLoader.h"
#include "Private/ImageSpNode.h"

#include <QQuickItem>
#include <QImage>

namespace sp {
/**
 * @brief Класс для отрисовки изображения с закруглёнными краями через Scene Graph.
 */
class ImageSp: public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (QString  source       READ source       WRITE setSource       NOTIFY sourceChanged)
    Q_PROPERTY (float    radius       READ radius       WRITE setRadius       NOTIFY radiusChanged)
    Q_PROPERTY (QSize    sourceSize   READ sourceSize                         NOTIFY sourceSizeChanged)
    Q_PROPERTY (FillMode fillMode     READ fillMode     WRITE setFillMode     NOTIFY fillModeChanged)
    Q_PROPERTY (Status   status       READ status                             NOTIFY statusChanged)
    Q_PROPERTY (bool     asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)

    /** @property mipmap
     *  @brief Флаг mipmap-фильтрации изображения, оно же более качественное масштабирование.
     *  @note  На iOS может глючить, нужно проверить
     */
    Q_PROPERTY (bool mipmap READ mipmap WRITE setMipmap NOTIFY mipmapChanged)

    Q_PROPERTY (HorizontalAlignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY (VerticalAlignment   verticalAlignment   READ verticalAlignment   WRITE setVerticalAlignment   NOTIFY verticalAlignmentChanged)

    public:
        enum FillMode {
            Stretch = 0
            ,PreserveAspectFit = 1
            ,PreserveAspectCrop = 2
            ,Pad = 6
            ,Parallax = 8
        };

        /** @brief Горизонтальное выравнивание */
        enum HorizontalAlignment {
            AlignLeft = 1
            , AlignRight = 2
            , AlignHCenter = 4
        };

        /** @brief Вертикальное выравнивание */
        enum VerticalAlignment {
            AlignTop = 32
            , AlignBottom = 64
            , AlignVCenter = 128
        };

        enum Status {
            Null = 0
            , Ready = 1
            , Loading = 2
            , Error = 3
        };

        Q_ENUM (FillMode)
        Q_ENUM (HorizontalAlignment)
        Q_ENUM (VerticalAlignment)
        Q_ENUM (Status)

    public:
        ImageSp(QQuickItem *parent = nullptr);
        ~ImageSp();

        virtual void classBegin() override;
        virtual void componentComplete() override;
        QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        const QString& source() const { return _source; }
        float    radius() const { return _radius; }
        QSize    sourceSize() const { return _image->size(); }
        FillMode fillMode() const { return _fillMode; }
        Status   status() const { return _status; }
        bool     asynchronous() const { return _asynchronous; }
        bool     mipmap() const { return _mipmap; }
        HorizontalAlignment horizontalAlignment() const { return _horizontalAlignment; }
        VerticalAlignment   verticalAlignment() const { return _verticalAlignment; }

        void setSource(const QString &source);
        void setRadius(float radius);
        void setFillMode(FillMode fillMode);
        void setAsynchronous (bool asynchronous);
        void setMipmap(bool mipmap);
        void setHorizontalAlignment (HorizontalAlignment horizontalAlignment);
        void setVerticalAlignment (VerticalAlignment verticalAlignment);

    signals:
        void sourceChanged(const QString&);
        void radiusChanged(float);
        void sourceSizeChanged(const QSize&);
        void statusChanged(Status);
        void fillModeChanged(FillMode);
        void asynchronousChanged(bool);
        void mipmapChanged(bool);
        void horizontalAlignmentChanged(HorizontalAlignment);
        void verticalAlignmentChanged(VerticalAlignment);

    protected slots:
        void onImageSpLoaded (const QString &source, sp::ImageWeakPtr image);
        void onImageSpError  (const QString &source, sp::ImageWeakPtr image, const QString &reason);

    protected:
        struct Coefficients {
            float x,y,w,h;
            float tx,ty,tw,th;
        };

        Coefficients coefficientsPad() const;
        Coefficients coefficientsStretch() const;
        Coefficients coefficientsPreserveAspectFit() const;
        Coefficients coefficientsPreserveAspectCrop() const;
        Coefficients coefficientsParallax() const;

    protected:
        bool _completed = false;
        int _vertexAtCorner = 20;
        int _segmentCount = 4*_vertexAtCorner+3;

        ImageSpNode *_node;
        ImageSharedPtr _image;
        bool _imageUpdated = false;

        QString _source;
        float  _radius = 0.0;
        Status _status = Null;
        bool _asynchronous = true;
        bool _mipmap = true;
        FillMode _fillMode = PreserveAspectCrop;
        HorizontalAlignment _horizontalAlignment = AlignHCenter;
        VerticalAlignment _verticalAlignment = AlignVCenter;
};
} // namespace sp {
