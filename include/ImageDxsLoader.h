#ifndef IMAGEDXSLOADER_H
#define IMAGEDXSLOADER_H

#include <QObject>
#include <QThread>
#include <QImage>

namespace dxs {

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
        void get (const QString &source, QImage *image);

    signals:
        /// @brief Загружает изображение ассинхронно.
        void loadTo(const QString &source, QImage *image);

        /// @brief [signal] Сигнал, уведомляющий о загрузке изображения.
        void loaded(const QString &source, QImage *image);

        /// @brief [signal] Сигнал об ошибке при загрузке изображения.
        void error(const QString &source, QImage *image, const QString &reason);

    private:
        ImageDxsLoader ();

    private:
        QThread _thread;
}; // class ImageDxsLoader

} // namespace dxs {

#endif // IMAGEDXSLOADER_H
