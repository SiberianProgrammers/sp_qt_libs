#include "Private/DownloadFileHandler.h"

#include <QFile>
#include <QStringBuilder>
#include <LogSp.h>

using namespace sp;

//------------------------------------------------------------------------------
DownloadFileHandler::DownloadFileHandler(const QString &fileName)
    : _fileName (fileName)
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

    LOG_ALEUS("onDownloadFileProgress" << (bytesReceived + quint64(context->file()->size())) << "/" << bytesTotal);
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    DownloadFileHandler *context = static_cast<DownloadFileHandler*>(reply->request().originatingObject());

    if (reply->error() == QNetworkReply::NoError) {
        LOG_ALEUS("Файл" << context->file()->fileName() << "загружен");
        context->file()->close();
    } else {
        LOG_ERROR("Ошибка загрузки файла");

        context->file()->flush();
        // Пока закрываем файл, но для возобновления закачки нужно оставлять открытым
        //context->file->close();
    }
}

//------------------------------------------------------------------------------
void DownloadFileHandler::onError(QNetworkReply::NetworkError)
{

}
