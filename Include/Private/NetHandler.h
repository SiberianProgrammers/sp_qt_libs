#pragma once

#include <QObject>
#include <QNetworkReply>
#include <QUrl>

namespace sp {

/***************************************************************************//**
 * @brief Абстрактный обработчик сетевого запроса
 ******************************************************************************/
class NetHandler: public QObject {
    Q_OBJECT

    protected:
        NetHandler();

    public:
        virtual const QUrl& url() const = 0;

        /// @brief Настравивает запрос перед отправкой
        virtual void tuningRequest(QNetworkRequest *) {}

    signals:
        /// @brief [signal] Сигнал об успешном выполенении сетевого запроса
        void finished();

        /// @brief [signal] Сигнал об ошибке запроса
        /// @param needRetry - нужен повтор запроса, после востановления связи
        void error(bool needRetry);

    protected slots:
        virtual void onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal) = 0;
        virtual void onFinished() = 0;
        virtual void onError(QNetworkReply::NetworkError) = 0;

    friend class Net;
};

} // namespace sp {
