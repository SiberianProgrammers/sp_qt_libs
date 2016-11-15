#ifndef NETWORKACCESSMANAGERWORKER_H
#define NETWORKACCESSMANAGERWORKER_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QTimer>
#include <QHttpMultiPart>

namespace sp {
/**
 * @brief Класс для загрузки статичных фалов из сети с поддержкой кеширования.
 * @note Экземпляр класс переносится в отдельный поток при создании.
 */
class NetworkAccessManagerWorker: public QObject
{
    Q_OBJECT

    public:
        explicit NetworkAccessManagerWorker();
        ~NetworkAccessManagerWorker();

    signals:
        void get (const QUrl &url);
        void post (const QUrl &url, QHttpMultiPart *multiPart);
        void finished (QNetworkReply *reply);
        void error(QNetworkReply *reply);

    private slots:
        void _get (const QUrl &url);
        void _post (const QUrl &url, QHttpMultiPart *multiPart);
        void _finished(QNetworkReply *reply);
        void sendRequestFromQueue();

    private:
        /**
         * @brief The NetworkRequest struct
         *        Класс - запрос, либо POST, либо GET
         *        Используется NetworkAccessManagerWorker
         */
        struct NetworkRequest
        {
            enum RequestMethod { POST, GET };

            NetworkRequest (const QUrl& url, RequestMethod method=GET, QHttpMultiPart *multipart=NULL);

            RequestMethod   _method;
            QNetworkRequest _request;
            QHttpMultiPart  *_multipart;
        };

        static QThread         _thread;
        QNetworkAccessManager  *_nam;
        QList<NetworkRequest *>*_requestQueue;
        QTimer                 *_reloadTimer;
        int                     _timoutInterval;

        const int _MAX_TIMEOUT;
        const int _MIN_TIMEOUT;
        const int _TIMEOUT_STEP;
};
} // namespace sp

#endif // NETWORKACCESSMANAGERWORKER_H

