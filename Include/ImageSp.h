#pragma once

#include <QtQuick>
#include <QImage>
#include <QString>

#include "ImageSpLoader.h"

namespace sp {
/**
 * @brief Класс QML-объекта изображения с закруглёнными углами. Должен работать быстро.
 */
class ImageSp : public QQuickPaintedItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY (QString  source       READ source       WRITE setSource       NOTIFY sourceChanged)
    Q_PROPERTY (double   radius       READ radius       WRITE setRadius       NOTIFY radiusChanged)
    Q_PROPERTY (QSize    sourceSize   READ sourceSize                         NOTIFY sourceSizeChanged)
    Q_PROPERTY (FillMode fillMode     READ fillMode     WRITE setFillMode     NOTIFY fillModeChanged)
    Q_PROPERTY (Status   status       READ status                             NOTIFY statusChanged)
    Q_PROPERTY (bool     antialiasing READ antialiasing WRITE setAntialiasing )
    Q_PROPERTY (bool     blur         READ blur         WRITE setBlur         NOTIFY blurChanged)
    Q_PROPERTY (bool     asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)

    Q_PROPERTY (HorizontalAlignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY (VerticalAlignment   verticalAlignment   READ verticalAlignment   WRITE setVerticalAlignment   NOTIFY verticalAlignmentChanged)

    public:
        enum FillMode {
            Stretch = 0
            ,PreserveAspectFit = 1
            ,PreserveAspectCrop = 2
            ,Pad = 6
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

        virtual void classBegin() override;
        virtual void componentComplete() override;
        virtual void paint(QPainter *painter) override;

        QString  source() const;
        double   radius() const;
        QSize    sourceSize() const;
        FillMode fillMode() const;
        Status   status() const;
        bool     blur() const;
        bool     asynchronous () const;
        HorizontalAlignment horizontalAlignment() const;
        VerticalAlignment   verticalAlignment() const;

        void setSource(const QString &source);
        void setRadius(double radius);
        void setFillMode(FillMode fillMode);
        void setBlur(bool blur);
        void setAsynchronous (bool asynchronous);
        void setHorizontalAlignment (HorizontalAlignment horizontalAlignment);
        void setVerticalAlignment (VerticalAlignment verticalAlignment);

    signals:
        void sourceChanged(const QString&);
        void radiusChanged(qreal);
        void sourceSizeChanged(const QSize&);
        void statusChanged(Status);
        void fillModeChanged(FillMode);
        void blurChanged(bool);
        void asynchronousChanged(bool);
        void horizontalAlignmentChanged(HorizontalAlignment);
        void verticalAlignmentChanged(VerticalAlignment);

    private:
        void drawPad (QPainter *painter, const QImage &image);
        void drawStretch (QPainter *painter, const QImage &image);
        void drawPreserveAcpectFit (QPainter *painter, const QImage &image);
        void drawPreserveAspectCrop (QPainter *painter, const QImage &image);

    private slots:
        void onImageSpLoaded (const QString &source, sp::WeakImage image);
        void onImageSpRendered (sp::WeakImage sourceImage, QImage renderImage);
        void onImageSpError  (const QString &source, sp::WeakImage image, const QString &reason);
        void renderImage();

    private:
        bool                _completed = false; // Флаг для инициализации объекта
        SharedImage         _image;
        QImage              _renderImage;
        QString             _source;
        double              _radius = 0.0;
        Status              _status = Null;
        FillMode            _fillMode = PreserveAspectCrop;
        bool                _blur = false;
        bool                _asynchronous = true;
        HorizontalAlignment _horizontalAlignment = AlignHCenter;
        VerticalAlignment   _verticalAlignment = AlignVCenter;
        bool                _inRender = false;
};
}// namespace experiment {
