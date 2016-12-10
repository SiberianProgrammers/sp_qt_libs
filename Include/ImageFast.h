#pragma once

#include <QQuickItem>
#include <QImage>
#include <SpImageNode.h>

#include "ImageSpLoader.h"

namespace sp {
/**
 * @brief Класс для отрисовки дуги через Scene Graph. Для более плавной отрисовки смотри Arc
 * @note Антиалиасинг осуществляется с помощью QSurfaceFormat format; format.setSample(16).
 *       Подробнее смотри в примере в файле main.cpp.
 */
class ImageFast : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (QString  source       READ source       WRITE setSource       NOTIFY sourceChanged)
    Q_PROPERTY (float    radius       READ radius       WRITE setRadius       NOTIFY radiusChanged)
    Q_PROPERTY (QSize    sourceSize   READ sourceSize                         NOTIFY sourceSizeChanged)
    Q_PROPERTY (FillMode fillMode     READ fillMode     WRITE setFillMode     NOTIFY fillModeChanged)
    Q_PROPERTY (Status   status       READ status                             NOTIFY statusChanged)
    Q_PROPERTY (bool     antialiasing READ antialiasing WRITE setAntialiasing )
    Q_PROPERTY (bool     asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)

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
        ImageFast(QQuickItem *parent = nullptr);
        ~ImageFast();

        virtual void classBegin() override;
        virtual void componentComplete() override;
        QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        const QString& source() const { return _source; }
        float    radius() const { return _radius; }
        QSize    sourceSize() const { return _image->size(); }
        FillMode fillMode() const { return _fillMode; }
        Status   status() const { return _status; }
        bool     asynchronous () const { return _asynchronous; }
        HorizontalAlignment horizontalAlignment() const { return _horizontalAlignment; }
        VerticalAlignment   verticalAlignment() const { return _verticalAlignment; }

        void setSource(const QString &source);
        void setRadius(float  radius);
        void setFillMode(FillMode fillMode);
        void setAsynchronous (bool asynchronous);
        void setHorizontalAlignment (HorizontalAlignment horizontalAlignment);
        void setVerticalAlignment (VerticalAlignment verticalAlignment);

    signals:
        void sourceChanged(const QString&);
        void radiusChanged(float);
        void sourceSizeChanged(const QSize&);
        void statusChanged(Status);
        void fillModeChanged(FillMode);
        void asynchronousChanged(bool);
        void horizontalAlignmentChanged(HorizontalAlignment);
        void verticalAlignmentChanged(VerticalAlignment);

    protected slots:
        void onImageSpLoaded (const QString &source, sp::WeakImage image);
        void onImageSpError  (const QString &source, sp::WeakImage image, const QString &reason);

    protected:
        struct Coefficients {
            float x,y,w,h;
            float tx,ty,tw,th;
        };

        Coefficients coefficientsPad() const;
        Coefficients coefficientsStretch() const;
        Coefficients coefficientsPreserveAspectFit() const;
        Coefficients coefficientsPreserveAspectCrop() const;
        Coefficients coefficientsRectParallax() const;

    protected:
        bool _completed = false;
        int _vertexAtCorner = 20;
        int _segmentCount = 4*_vertexAtCorner+3;

        SpImageNode *_node;
        SharedImage _image;

        QString _source;
        float  _radius = 0.0;
        Status _status = Null;
        bool _asynchronous = true;
        FillMode _fillMode = PreserveAspectCrop;
        HorizontalAlignment _horizontalAlignment = AlignHCenter;
        VerticalAlignment _verticalAlignment = AlignVCenter;
};
} // namespace sp {
