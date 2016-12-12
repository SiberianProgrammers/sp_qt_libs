#pragma once

#include <QDebug>
#include <QObject>
#include <QTime>
#include <QStringList>

#ifdef SP_ALEUS
    #define LOG_ALEUS(str) qDebug() << str
#else
    #define LOG_ALEUS(str) ;
#endif

#ifdef SP_VONABIRG
    #define LOG_VONABIRG(str) qDebug() << str
#else
    #define LOG_VONABIRG(str)
#endif
#define LOG_V LOG_VONABIRG

// Depricated!!!
#define LOG_DEBUG(str) qDebug() << str
#define LOG_INFO(str)  qDebug() << str
#define LOG_ERROR(str) qCritical() << str
#define LOG_WARN(str)  qWarning() << str
#define LOG_FATAL(str) qFatal(str.toLocal8Bit())

namespace sp {
/**
 * @brief Класс логирования отладки, ошибок, предупреждений и т.п. Имеет интерфейс для C++ (signleton) и QML.
 */
class Log: public QObject
{
    Q_OBJECT

    Log(QObject *parent = NULL);

    public:
        static Log& instance();

        /** @brief Выводит в лог строку str. */
        // Depricated!!!
        Q_INVOKABLE static void debug   (const QString &str);
        Q_INVOKABLE static void info    (const QString &str);
        Q_INVOKABLE static void error   (const QString &str);
        Q_INVOKABLE static void warning (const QString &str);
        Q_INVOKABLE static void fatal   (const QString &str);
        Q_INVOKABLE void aleus    (const QString &str);
        Q_INVOKABLE void v (const QString &str);

        /// @brief Возвращает архив логирования
        Q_INVOKABLE QString logArchive() const;

        static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

    signals:
        void logArchiveChanged();

    private:
        inline void toArchive(const QString &str);

    private:
        static QStringList _logArchive;
        static Log         _instance;
        static QTime       _time;
}; // class Log: public QObject
} // namespace sp {
