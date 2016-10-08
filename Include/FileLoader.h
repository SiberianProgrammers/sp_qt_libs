#ifndef STATICFILELOADER_H
#define STATICFILELOADER_H

#include <QObject>
#include <QQmlParserStatus>
#include "NetworkAccessManagerWorker.h"

namespace dxs {
/**
 * @brief Класс для загрузки статичных фалов из сети с поддержкой кеширования.
 * @note Экземпляр класс переносится в отдельный поток при создании.
 */
class FileLoader: public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY (QString fileName        MEMBER _fileName)
    Q_PROPERTY (QString url             MEMBER _url)
    Q_PROPERTY (int     version         MEMBER _version)
    Q_PROPERTY (QString fileContent     MEMBER _fileContent NOTIFY fileReady)
    Q_PROPERTY (QString errorText       MEMBER _error       NOTIFY errorReady)
    Q_PROPERTY (bool    forceUseNetwork MEMBER _forceUseNetwork)

    public:
        FileLoader(QObject *parent = NULL);
        ~FileLoader();

        /**
         * @brief Запрашивает файл с сервера или из кеша. Ответ приходит по сигналу responseRecieved.
         *
         * @param fileName наименование файла
         * @param url адрес файла на сервере
         * @param version версия запрашиваемого файла
         */
        Q_INVOKABLE void requestFile (const QString &fileName, const QString &url, int version);
        Q_INVOKABLE void requestFile ();

        /**
          * @brief Записывает содержимое в файл.
          *
          * @param fileName наименование файла
          * @param content содержимое файла для записи
          */
        Q_INVOKABLE void writeToFile (const QString &fileName, const QString &content);

        /**
          * @brief Читает содержимое из файла, возвращает пустую строку если не удалось прочитать.
          *
          * @param fileName наименование файла
          * @param content содержимое файла для записи
          */
        Q_INVOKABLE QString readFromFile (const QString &fileName);

        /**
          * @brief Очищает файл кеша.
          */
        Q_INVOKABLE void clearCache();

        virtual void classBegin();
        virtual void componentComplete();

    signals:
        /**
         * @brief Сигнал о получения файла.
         * @param fileContent содержимое файла
         */
        void fileReady ();

        /**
         *  @brief Сигнал о невозможности получения файла (критическая ошибка)
         */
        void errorReady ();

    public slots:
        /**
         * @brief Обрабатывает закрытие http-запроса.
         */
        void finishedRequest (QNetworkReply *reply);

        /**
         * @brief Закрытие http-запроса с ошибкой
         */
        void errorRequest(QNetworkReply *replyWithError);

    private:
        NetworkAccessManagerWorker _nam;
        QString                    _fileContent;
        QString                    _fileName;
        QString                    _url;
        QString                    _error;
        int                        _version;
        bool                       _forceUseNetwork;
};
} //namespace DXS

#endif // STATICFILELOADER_H

