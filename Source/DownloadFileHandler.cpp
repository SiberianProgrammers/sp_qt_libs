#include "DownloadFileHandler.h"

#include <LogSp.h>
#include <QFile>
#include <QStringBuilder>
#include <QThread>

using namespace sp;

//------------------------------------------------------------------------------
DownloadFileHandler::DownloadFileHandler(const QUrl &url, const QString &fileName)
    : _url(url)
    , _fileName (fileName)
{
    //TODO Добавить путь к сохранённым файлам
    //TODO Заменить ~ на ., чтобы файл был скрытым
    _file = new QFile('~' % _fileName);
    if (_file->exists()) {
        _file->remove();
    }
}

//------------------------------------------------------------------------------
void DownloadFileHandler::tuningRequest(QNetworkRequest *request)
{
    QByteArray rangeValue = "bytes=" + QByteArray::number(_file->size()) + "-";
    request->setRawHeader("Range", rangeValue);
    _needCheckAcceptRanges = true;
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onDownloadProgress(quint64 /*bytesReceived*/, quint64 /*bytesTotal*/)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(reply->request().originatingObject());

    if (!reply->isOpen()) {
        // Этот случай возникает, если отменить запрос в onDownloadProgress
        return;
    }

    if (_needCheckAcceptRanges) {
        bool acceptRanges = reply->rawHeader("Content-Range") != "";

        if (acceptRanges) {
            _file->open(QIODevice::Append);
        } else {
            _file->open(QIODevice::WriteOnly);
        }

        _needCheckAcceptRanges = false;
    }

    QByteArray buf = reply->readAll();
    context->file()->write(buf);
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(reply->request().originatingObject());

    if (reply->error() == QNetworkReply::NoError) {
        context->file()->close();
        if (QFile::exists(context->fileName())) {
            QFile::remove(context->fileName());
        }
        context->file()->rename(context->fileName());
        emit finished();
    }
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onError(QNetworkReply::NetworkError code)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(reply->request().originatingObject());

    if (code == QNetworkReply::ContentNotFoundError
      ||code == QNetworkReply::ContentAccessDenied
      ||code == QNetworkReply::ContentOperationNotPermittedError
      ||code == QNetworkReply::ProtocolUnknownError
      ||code == QNetworkReply::AuthenticationRequiredError)
    {
        LOG_ERROR("Ошбика загрузки файла. Тупо нельзя его скачать" << reply->url().toString());
        context->file()->close();

        if (context->file()->exists()) {
            context->file()->remove();
        }

        emit error(false);
    } else {
        if (code != QNetworkReply::OperationCanceledError) {
            LOG_ERROR("Некритичная ошибка загрузки файла - повтор" << reply->url().toString());
        }

        context->file()->close();
        emit error(true);
    }
}
