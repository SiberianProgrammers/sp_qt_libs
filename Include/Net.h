#pragma once

#include "Private/NetHandler.h"
#include "DownloadFileHandler.h"

#include <QObject>
#include <QFile>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>
#include <QQueue>
#include <QSet>

namespace sp {

/**
 * @brief Синглетон для работы с сетью. Все запросы проводит через параллельный поток.
 */
class Net: public QObject {
    Q_OBJECT

    Net();
    public:
        static Net& instance();
        DownloadFileHandler* downloadFile(const QString &url, const QString &fileName);

    signals:
        void makeRequest(NetHandler *handler);

    protected slots:
        void onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility accessible);
        void onMakeRequest(NetHandler *handler = nullptr);
        void onNetHandlerFinished();
        void onNetHandlerError(bool needRetry);

    protected:
        QNetworkAccessManager _nam;
        QThread _thread;
        QSet<NetHandler *> _activeHandler;   // Активные сетевые запросы. Максимальное количество - MAX_ACTIVE_HANDLERS
        QQueue<NetHandler *> _handlersQueue; // Очередь обработчиков сетевых запросов
                                             // TODO Для очереди приоритетов нужно сделать отдельный контейнер
};
};
