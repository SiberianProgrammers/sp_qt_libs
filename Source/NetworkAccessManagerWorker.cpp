#include <qDebug>
#include "NetworkAccessManagerWorker.h"

//--------------------------------------------------------------------------------
QThread dxs::NetworkAccessManagerWorker::_thread;
//--------------------------------------------------------------------------------
dxs::NetworkAccessManagerWorker::NetworkAccessManagerWorker()
    : QObject(NULL)
    , _nam(new QNetworkAccessManager(NULL))
    , _requestQueue(new QList<NetworkRequest *>)
    , _reloadTimer(new QTimer(NULL))
    , _MAX_TIMEOUT(800)
    , _MIN_TIMEOUT(0)
    , _TIMEOUT_STEP(200)
{
    if (!_thread.isRunning()) {
        _thread.start();
    }
    _nam->moveToThread(&_thread);
    _reloadTimer->moveToThread(&_thread);
    this->moveToThread(&_thread);

    connect (this, SIGNAL(get(QUrl)), SLOT(_get(QUrl)));
    connect (this, SIGNAL(post(QUrl,QHttpMultiPart*)), SLOT(_post(QUrl,QHttpMultiPart*)));
    connect (_nam, SIGNAL(finished(QNetworkReply *)), SLOT(_finished(QNetworkReply *)));

    _timoutInterval = _MIN_TIMEOUT;
    //_requestShotCount = 0;
    _reloadTimer->setSingleShot(true);
    _reloadTimer->setInterval(_timoutInterval);

    connect(_reloadTimer, SIGNAL(timeout()), SLOT(sendRequestFromQueue()));
}

//--------------------------------------------------------------------------------
dxs::NetworkAccessManagerWorker::~NetworkAccessManagerWorker()
{
    _nam->deleteLater();
    _reloadTimer->deleteLater();
}

//--------------------------------------------------------------------------------
void dxs::NetworkAccessManagerWorker::_get(const QUrl &url)
{
    // Добавляем запрос в очередь, если это первый запрос - то тут же его выполняем
    _requestQueue->append (new NetworkRequest(url, NetworkRequest::GET));

    if (_requestQueue->count() == 1) {
        sendRequestFromQueue();
    }
}

//--------------------------------------------------------------------------------
void dxs::NetworkAccessManagerWorker::_post(const QUrl &url, QHttpMultiPart *multiPart)
{
    // Добавляем запрос в очередь, если это первый запрос - то тут же его выполняем
    _requestQueue->append (new NetworkRequest(url, NetworkRequest::POST, multiPart));

    if (_requestQueue->count() == 1) {
        sendRequestFromQueue();
    }
}

//--------------------------------------------------------------------------------
void dxs::NetworkAccessManagerWorker::_finished(QNetworkReply *reply)
{
    // Если нет ошибок передаем дальше, если есть ошибка - обрабатываем ее
    if (reply->error() == QNetworkReply::NoError) {
        // Удаляем запрос из очереди
        delete _requestQueue->takeFirst();

        // сбрасываем таймер
        _timoutInterval = _MIN_TIMEOUT;

        // Завершаем запрос
        emit finished(reply);

        // Начинаем загрузку следующего ресурса
        sendRequestFromQueue();

        return ;
    }

    // Есть ошибка смотрим, что за ошибка, ошибки с отстутствием доступа к контенту недопустимы
    if (   reply->error() == QNetworkReply::ContentNotFoundError
        || reply->error() == QNetworkReply::ContentAccessDenied
        || reply->error() == QNetworkReply::ContentOperationNotPermittedError
        || reply->error() == QNetworkReply::ProtocolUnknownError
        || reply->error() == QNetworkReply::AuthenticationRequiredError)
    {
        qDebug() << "NetworkAccessManagerWorker: " << reply->errorString() << "(" << reply->request().url().url() << ")";

        //Сбрасываем таймер и повторы
        _timoutInterval = _MIN_TIMEOUT;
        //_requestShotCount = 0;

        // генерим ошибку
        emit error(reply);

        // Удаляем порченный запрос из очереди
        if (!_requestQueue->isEmpty()) {
            delete _requestQueue->takeFirst();
            sendRequestFromQueue();
        }

        // Отправляем следующий запрос без задержки
        return ;
    }

    // В любом другом случае,запускаем таймер перезагрузки
    if (_timoutInterval < _MAX_TIMEOUT) {
        _timoutInterval += _TIMEOUT_STEP;
    }

    //_requestShotCount += 1;
    _reloadTimer->setInterval(_timoutInterval);
    _reloadTimer->start(_timoutInterval);
}

//--------------------------------------------------------------------------------
void dxs::NetworkAccessManagerWorker::sendRequestFromQueue()
{
    if (_requestQueue->isEmpty()) {
        return ;
    }

    NetworkRequest *r = _requestQueue->first();
    if (r == NULL) {
        return;
    }

    if (r->_method == NetworkRequest::GET) {
        _nam->get( r->_request );
    } else if (r->_method == NetworkRequest::POST) {
        QNetworkReply* reply = _nam->post(r->_request, r->_multipart);

        if (r->_multipart != NULL) {
            r->_multipart->setParent(reply);
        }
    }
}

//--------------------------------------------------------------------------
dxs::NetworkAccessManagerWorker::NetworkRequest::NetworkRequest(const QUrl &url, NetworkAccessManagerWorker::NetworkRequest::RequestMethod method, QHttpMultiPart *multipart) :
    _method(method)
  , _request(url)
  , _multipart(multipart)
{
}
