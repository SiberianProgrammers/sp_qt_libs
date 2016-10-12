#pragma once

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QImage>

namespace dxs {
typedef QSharedPointer<QImage> SharedImage;
typedef QWeakPointer<QImage> WeakImage;

/***************************************************************************//**
 * @brief Класс-singleton загрузщика изображений для ImageDxs.
 *
 * @note Работает в параллельном потоке.
 ******************************************************************************/
class ImageDxsLoader : public QObject {
    Q_OBJECT

    public:
        static ImageDxsLoader& instance();

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
        ImageDxsLoader ();

    private:
        QThread _thread;
}; // class ImageDxsLoader
} // namespace dxs {

Q_DECLARE_METATYPE(dxs::SharedImage)
Q_DECLARE_METATYPE(dxs::WeakImage)
