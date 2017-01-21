#pragma once

#include "Private/NetHandler.h"

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QFileInfo>

class QFile;

namespace sp {

/***************************************************************************//**
 * @brief Обработчик запроса на скачивание файла
 ******************************************************************************/
class DownloadFileHandler: public NetHandler {
    Q_OBJECT

    public:
        DownloadFileHandler(const QUrl &url, const QFileInfo &fileInfo);

        virtual const QUrl& url() const override { return _url; }
        virtual void tuningRequest(QNetworkRequest *) override;

        const QFileInfo& fileInfo() const { return _fileInfo; }
        QFile *file() { return  _file; }

    protected slots:
        virtual void onError(QNetworkReply::NetworkError) override;
        virtual void onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal) override;
        virtual void onFinished() override;

    protected:
        QUrl _url;
        QFileInfo _fileInfo;
        QFile *_file;
        bool _needCheckAcceptRanges;
};

} // namespace sp {
