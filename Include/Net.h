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
        void onMakeRequest(NetHandler *handler);

    protected:
        QNetworkAccessManager _nam;
        QThread _thread;
        QQueue<NetHandler *> _handlers; // Обработчики сетевых запросов
                                        // TODO Для очереди приоритетов нужно сделать отдельный контейнер
};
};
