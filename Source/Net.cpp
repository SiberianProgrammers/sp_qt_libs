#include "Net.h"

#include <QNetworkReply>
#include <QStringBuilder>
#include <LogSp.h>

using namespace sp;

//------------------------------------------------------------------------------
Net::Net()
    : QObject(nullptr)
{
    _nam.moveToThread(&_thread);
    moveToThread(&_thread);
    _thread.start();

    connect (this, &Net::makeRequest, this, &Net::onMakeRequest);
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

//--------------------------------------------------------------------------
void Net::onMakeRequest(NetHandler *handler)
{
    QNetworkRequest request;
    request.setUrl(QUrl(handler->url()));
    request.setOriginatingObject(handler);

    QNetworkReply *reply = _nam.get(request);
    connect (reply, &QNetworkReply::downloadProgress, handler, &NetHandler::onDownloadProgress);
    connect (reply, &QNetworkReply::finished, handler, &NetHandler::onFinished);
    connect (reply, SIGNAL(error(QNetworkReply::NetworkError)), handler, SLOT(onError(QNetworkReply::NetworkError)));
}
