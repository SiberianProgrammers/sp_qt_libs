#pragma once

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QImage>

namespace sp {
typedef QSharedPointer<QImage> ImageSharedPtr;
typedef QWeakPointer<QImage> ImageWeakPtr;

/***************************************************************************//**
 * @brief Класс-singleton загрузщика изображений для ImageSp.
 *
 * @note Работает в параллельном потоке.
 ******************************************************************************/
class ImageSpLoader : public QObject {
    Q_OBJECT

    ImageSpLoader ();
    public:
        static ImageSpLoader& instance();

    public slots:
        /// @brief Загружает изображение синхронно.
        void get (const QString &source, ImageSharedPtr image);

    signals:
        /// @brief Загружает изображение ассинхронно.
        void loadTo(const QString &source, ImageSharedPtr image);

        /// @brief [signal] Сигнал, уведомляющий о загрузке изображения.
        void loaded(const QString &source, ImageWeakPtr image);

        /// @brief [signal] Сигнал об ошибке при загрузке изображения.
        void error(const QString &source, ImageWeakPtr image, const QString &reason);

    private:
        QThread _thread;
}; // class ImageSpLoader
} // namespace sp {

Q_DECLARE_METATYPE(sp::ImageSharedPtr)
Q_DECLARE_METATYPE(sp::ImageWeakPtr)
