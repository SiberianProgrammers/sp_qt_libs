#include "Net.h"
#include "Private/DownloadFileHandler.h"

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

    connect (this, &Net::downloadFile, this, &Net::onDownloadFile);
}

//------------------------------------------------------------------------------
Net &Net::instance()
{
    static Net net;
    return net;
}

//------------------------------------------------------------------------------
void Net::onDownloadFile(const QString &url, const QString &fileName)
{
    DownloadFileHandler *handler = new DownloadFileHandler(fileName);
    makeRequest(url, handler);
}

//--------------------------------------------------------------------------
void Net::makeRequest(const QString &url, NetHandler *handler)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setOriginatingObject(handler);

    QNetworkReply *reply = _nam.get(request);
    connect (reply, &QNetworkReply::downloadProgress, handler, &NetHandler::onDownloadProgress);
    connect (reply, &QNetworkReply::finished, handler, &NetHandler::onFinished);
    connect (reply, SIGNAL(error(QNetworkReply::NetworkError)), handler, SLOT(onError(QNetworkReply::NetworkError)));
}
