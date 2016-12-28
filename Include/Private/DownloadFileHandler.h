#pragma once

#include "Private/NetHandler.h"

#include <QObject>
#include <QString>

class QFile;

namespace sp {

/// @brief Обработчик запроса на скачивание файла
class DownloadFileHandler: public NetHandler {
    Q_OBJECT

    public:
        DownloadFileHandler(const QString &fileName);

        const QString &fileName() const { return _fileName; }
        QFile *file() { return  _file; }

    public slots:
        virtual void onError(QNetworkReply::NetworkError) override;
        virtual void onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal) override;
        virtual void onFinished() override;

    protected:
        QString _fileName;
        QFile *_file;
};

} // namespace sp {
