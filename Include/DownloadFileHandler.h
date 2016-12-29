#pragma once

#include "Private/NetHandler.h"

#include <QObject>
#include <QString>
#include <QSharedPointer>

class QFile;

namespace sp {

/***************************************************************************//**
 * @brief Обработчик запроса на скачивание файла
 ******************************************************************************/
class DownloadFileHandler: public NetHandler {
    Q_OBJECT

    public:
        DownloadFileHandler(const QUrl &url, const QString &fileName);

        virtual const QUrl& url() const override { return _url; }
        const QString& fileName() const { return _fileName; }
        QFile *file() { return  _file; }

    protected slots:
        virtual void onError(QNetworkReply::NetworkError) override;
        virtual void onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal) override;
        virtual void onFinished() override;

    protected:
        QUrl _url;
        QString _fileName;
        QFile *_file;
};

typedef QSharedPointer<DownloadFileHandler> DownloadFileHandlerSharedPtr;
typedef QWeakPointer<DownloadFileHandler> DownloadFileHandlerWeakPtr;

} // namespace sp {


Q_DECLARE_METATYPE(sp::DownloadFileHandlerSharedPtr)
Q_DECLARE_METATYPE(sp::DownloadFileHandlerWeakPtr)
