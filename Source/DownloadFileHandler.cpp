#include "DownloadFileHandler.h"

#include <QFile>
#include <QStringBuilder>
#include <LogSp.h>

using namespace sp;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
int __idDownloadFileHandlerSharedPtr = qRegisterMetaType<DownloadFileHandlerSharedPtr>("DownloadFileHandlerSharedPtr");
int __idDownloadFileHandlerWeakPtr = qRegisterMetaType<DownloadFileHandlerWeakPtr>("DownloadFileHandlerWeakPtr");
#pragma clang diagnostic pop

//------------------------------------------------------------------------------
DownloadFileHandler::DownloadFileHandler(const QUrl &url, const QString &fileName)
    : _url(url)
    , _fileName (fileName)
{
    //TODO Добавить путь к сохранённым файлам
    _file = new QFile('~' % _fileName);
    _file->open(QIODevice::WriteOnly);
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onDownloadProgress(quint64 bytesReceived, quint64 bytesTotal)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(static_cast<QNetworkReply*>(sender())->request().originatingObject());
    context->file()->write(reply->readAll());

    LOG_ALEUS("onDownloadFileProgress" << bytesReceived << "/" << bytesTotal);
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(reply->request().originatingObject());

    if (reply->error() == QNetworkReply::NoError) {
        LOG_ALEUS("Файл" << context->file()->fileName() << "загружен");

        context->file()->close();
        emit finished();
    } else if (reply->error() == QNetworkReply::ContentNotFoundError
            || reply->error() == QNetworkReply::ContentAccessDenied
            || reply->error() == QNetworkReply::ContentOperationNotPermittedError
            || reply->error() == QNetworkReply::ProtocolUnknownError
            || reply->error() == QNetworkReply::AuthenticationRequiredError)
    {
        LOG_ERROR("Ошбика загрузки файла. Тупо нельзя его скачать" << reply->url().toString());
        context->file()->close();

        if (context->file()->exists()) {
            context->file()->remove();
        }

        emit error(false);
    } else {
        LOG_ERROR("Некритичная ошибка загрузки файла - повтор" << reply->url().toString());

        context->file()->flush();
        emit error(true);
    }
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onError(QNetworkReply::NetworkError code)
{
    LOG_ERROR ("DownloadFileHandler::onError" << code);
}
