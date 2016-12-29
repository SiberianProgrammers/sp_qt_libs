#include "Net.h"

#include <QNetworkReply>
#include <QStringBuilder>
#include <LogSp.h>

using namespace sp;

const int MAX_ACTIVE_HANDLERS = 5;

//------------------------------------------------------------------------------
Net::Net()
    : QObject(nullptr)
{
    _nam.moveToThread(&_thread);
    moveToThread(&_thread);
    _thread.start();

    connect (this, &Net::makeRequest, this, &Net::onMakeRequest);
    connect (&_nam, &QNetworkAccessManager::networkAccessibleChanged, this, &Net::onNetworkAccessibilityChanged);
}

//------------------------------------------------------------------------------
Net &Net::instance()
{
    static Net net;
    return net;
}

//------------------------------------------------------------------------------
DownloadFileHandler* Net::downloadFile(const QString &url, const QString &fileName)
{
    DownloadFileHandler *handler = new DownloadFileHandler(url, fileName);
    emit makeRequest(handler);

    return handler;
}

//------------------------------------------------------------------------------
void Net::onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
    if (accessible == QNetworkAccessManager::Accessible) {
        onMakeRequest();
    }
}

//--------------------------------------------------------------------------
void Net::onMakeRequest(NetHandler *handler/* = nullptr*/)
{
    while (_activeHandler.count() < MAX_ACTIVE_HANDLERS && _nam.networkAccessible() == QNetworkAccessManager::Accessible) {
        if (!handler) {
            if (!_handlersQueue.isEmpty()) {
                handler = _handlersQueue.dequeue();
            } else {
                // Очередь запрос пуста
                return;
            }
        }

        _activeHandler << handler;

        QNetworkRequest request;
        request.setUrl(QUrl(handler->url()));
        request.setOriginatingObject(handler);

        QNetworkReply *reply = _nam.get(request);
        connect (reply, &QNetworkReply::downloadProgress, handler, &NetHandler::onDownloadProgress);
        connect (reply, &QNetworkReply::finished, handler, &NetHandler::onFinished);
        connect (reply, SIGNAL(error(QNetworkReply::NetworkError)), handler, SLOT(onError(QNetworkReply::NetworkError)));

        connect (handler, &NetHandler::finished, this, &Net::onNetHandlerFinished);
        connect (handler, &NetHandler::error, this, &Net::onNetHandlerError);

        handler = nullptr;
    } // while (_activeHandler.count() < MAX_ACTIVE_HANDLERS) {

    // Если уже выполняется максимальное количество запросов
    if (handler) {
        _handlersQueue << handler;
    }
}

//------------------------------------------------------------------------------
void Net::onNetHandlerFinished()
{
    NetHandler *handler = static_cast<NetHandler *>(sender());
    _activeHandler.remove(handler);
}

//------------------------------------------------------------------------------
void Net::onNetHandlerError(bool needRetry)
{
    NetHandler *handler = static_cast<NetHandler *>(sender());
    _activeHandler.remove(handler);

    if (needRetry) {
        _handlersQueue.enqueue(handler);
    }
}
