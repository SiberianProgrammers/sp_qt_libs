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

    signals:
        /// @brief Загружает изображение ассинхронно.
        void loadTo(const QString &source, SharedImage image);

        /// @brief [signal] Сигнал, уведомляющий о загрузке изображения.
        void loaded(const QString &source, WeakImage image);

        /// @brief [signal] Сигнал об ошибке при загрузке изображения.
        void error(const QString &source, WeakImage image, const QString &reason);

    private:
        ImageSpLoader ();

    private:
        QThread _thread;
}; // class ImageSpLoader
} // namespace sp {

Q_DECLARE_METATYPE(sp::SharedImage)
Q_DECLARE_METATYPE(sp::WeakImage)
