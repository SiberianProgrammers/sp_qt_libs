#pragma once

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QImage>

namespace sp {
typedef QSharedPointer<QImage> SharedImage;
typedef QWeakPointer<QImage> WeakImage;

/***************************************************************************//**
 * @brief Класс-singleton загрузщика изображений для ImageSp.
 *
 * @note Работает в параллельном потоке.
 ******************************************************************************/
class ImageSpLoader : public QObject {
    Q_OBJECT

    public:
        static ImageSpLoader& instance();

    public slots:
        /// @brief Загружает изображение синхронно.
        void get (const QString &source, SharedImage image);

    private slots:
        void onRenderImageCrop (SharedImage sourceImage, qreal w, qreal h, qreal radius);

    signals:
        /// @brief Загружает изображение ассинхронно.
        void loadTo(const QString &source, SharedImage image);

        /// @brief Отрисовывает изображение в нужных размерах. PreserveAspectCrop.
        void renderImageCrop (SharedImage sourceImage, qreal width, qreal height, qreal radius);

        /// @brief [signal] Сигнал, уведомляющий о загрузке изображения.
        void loaded(const QString &source, WeakImage image);

        /// @brief [signal] Сигнал об ошибке при загрузке изображения.
        void error(const QString &source, WeakImage image, const QString &reason);

        /// @brief [signal] Изображение отрисовалось в нужных размерах.
        void rendered(WeakImage sourceImage, QImage renderImage);

    private:
        ImageSpLoader ();

    private:
        QThread _thread;
}; // class ImageSpLoader
} // namespace sp {

Q_DECLARE_METATYPE(sp::SharedImage)
Q_DECLARE_METATYPE(sp::WeakImage)
