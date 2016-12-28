#pragma once

#include "Private/NetHandler.h"

#include <QObject>
#include <QFile>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>

namespace sp {

/**
 * @brief Синглетон для работы с сетью. Все запросы проводит через параллельный поток.
 */
class Net: public QObject {
    Q_OBJECT

    Net();
    public:
        static Net& instance();

    signals:
        QFile* downloadFile(const QString &url, const QString &fileName);
        void downloadComplete(const QString &fileName);

    protected slots:
        void onDownloadFile(const QString &url, const QString &fileName);

    protected:
        void makeRequest(const QString &url, NetHandler *handler);

    protected:
        QNetworkAccessManager _nam;
        QThread _thread;
};
};
