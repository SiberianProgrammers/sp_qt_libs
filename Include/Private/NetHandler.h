#pragma once

#include <QObject>
#include <QNetworkReply>

namespace sp {

/// @brief Абстрактный обработчик сетевого запроса
class NetHandler: public QObject {
    public slots:
        virtual void onError(QNetworkReply::NetworkError) = 0;
        virtual void onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal) = 0;
        virtual void onFinished() = 0;
};

} // namespace sp {
