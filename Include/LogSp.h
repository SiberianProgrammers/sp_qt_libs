#pragma once

#include <QDebug>
#include <QObject>
#include <QTime>
#include <QStringList>

#ifdef SP_ALEUS
    #define LOG_ALEUS(str) sp::Log::debug(str)
#else
    #define LOG_ALEUS(str) ;
#endif

#ifdef SP_VONABIRG
    #define LOG_VONABIRG(str) sp::Log::debug(str)
#else
    #define LOG_VONABIRG(str)
#endif

#define LOG_DEBUG(str) sp::Log::debug(str)
#define LOG_INFO(str)  sp::Log::info(str)
#define LOG_ERROR(str) sp::Log::error(str)
#define LOG_WARN(str)  sp::Log::warning(str)
#define LOG_FATAL(str) sp::Log::fatal(str)

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

        //TODO Переделать согласно qthelp://org.qt-project.qtdoc.580/qtdoc/debug.html#environment-variables-recognized-by-qt

        /** @brief Выводит в лог строку str. */
        Q_INVOKABLE static void debug   (const QString &str);
        Q_INVOKABLE static void info    (const QString &str);
        Q_INVOKABLE static void error   (const QString &str);
        Q_INVOKABLE static void warning (const QString &str);
        Q_INVOKABLE static void fatal   (const QString &str);

        Q_INVOKABLE void aleus    (const QString &str);
        Q_INVOKABLE void vonabirg (const QString &str);

        /// @brief Возвращает архив логирования
        Q_INVOKABLE QString logArchive() const;

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
