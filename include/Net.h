#pragma once

#include "Private/NetHandler.h"
#include "DownloadFileHandler.h"

#include <QObject>
#include <QFile>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>
#include <QQueue>
#include <QSet>

namespace sp {

/***************************************************************************//**
 * @brief Синглетон для работы с сетью.
 *
 * Класс отправляет запросы на сервер и в случае обрыва и восстановления связи
 * перепосылает неудавшиеся запросы. Класс является потокобезопасным и обработку
 * запросов ведёт в параллельном потоке.
 *
 * Для обработки запросов применяются дочерний классы NetHandler'а. При успешном
 * выполнении запроса посылается сигнал NetHandler::finished. В случае ошибки
 * отправляется сигнал NetHandler::error с аргументом needRetry. Если needRetry
 * выставлен в true, то запрос будет заново отправлен в свою очередь.
 *
 * Для отправки запроса нужно послать сигнал makeRequest с указателем на обработчик
 * запросов (дочерний класс NetHandler). Также имеютсю удобные методы для загрузки
 * файлов - downloadFile.
 *
 * Метод abortRequest останавливает обработку запроса.
 *
 * @note Временем жизни объектов NetHandler класс Net не управляет. Поэтому можно
 * удалять обработчики только после сигнала NetHandler::finished или
 * NetHandler::error(false). Чтобы удалить NetHandler в любое другое время нужно
 * запустить метод abortRequest.
 *
 * @code
 * DownloadFileHandler *handler = Net::downloadFile("http://download.downloadmaster.ru/dm/dmaster.exe");
 * QObject::connect(handler, &DownloadFileHandler::finished , [=](){
 *     LOG_ALEUS("main.cpp: Загрузка завершена" << handler->file()->size());
 * } );
 ******************************************************************************/
class Net: public QObject {
    Q_OBJECT

    Net();
    public:
        static Net& instance();

        static DownloadFileHandler* downloadFile(const QString &url, const QString &fileName);
        static DownloadFileHandler* downloadFile(const QString &url, const QFileInfo &fileName);
        static DownloadFileHandler* downloadFile(const QString &url);

        static QThread* thread() { return &instance()._thread; }

    signals:
        /// @brief Посылает запрос и передаёт его обработку handler'у
        void makeRequest(NetHandler *handler);

        /// @brief Останавливает обработку запроса, соответствующего handler'у
        void abortRequest(NetHandler *handler);

    protected slots:
        void onNetworkAccessibilityChanged(QNetworkAccessManager::NetworkAccessibility accessible);
        void onMakeRequest(NetHandler *handler = nullptr);
        void onNetHandlerFinished();
        void onNetHandlerError(bool needRetry);
        void onAbortRequest(NetHandler *handler);

    protected:
        QNetworkAccessManager _nam;
        QThread _thread;
        QSet<NetHandler *> _activeHandler;   // Активные сетевые запросы. Максимальное количество - MAX_ACTIVE_HANDLERS
        QQueue<NetHandler *> _handlersQueue; // Очередь обработчиков сетевых запросов
                                             // TODO Для очереди приоритетов нужно сделать отдельный контейнер
};
};
