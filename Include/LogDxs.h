#pragma once

#include <QObject>
#include <QTime>
#include <QStringList>

#ifdef DXS_ALEUS
    #define LOG_ALEUS(str) dxs::Log::debug(str)
#else
    #define LOG_ALEUS(str) ;
#endif

#ifdef DXS_DED
    #define LOG_DED(str) dxs::Log::debug(str)
#else
    #define LOG_DED(str) ;
#endif

#ifdef DXS_VONABIRG
    #define LOG_VONABIRG(str) dxs::Log::debug(str)
#else
    #define LOG_VONABIRG(str)
#endif

#define LOG_DEBUG(str) dxs::Log::debug(str)
#define LOG_INFO(str)  dxs::Log::info(str)
#define LOG_ERROR(str) dxs::Log::error(str)
#define LOG_WARN(str)  dxs::Log::warning(str)
#define LOG_FATAL(str) dxs::Log::fatal(str)

namespace dxs {
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
        Q_INVOKABLE static void debug   (const QString &str);
        Q_INVOKABLE static void info    (const QString &str);
        Q_INVOKABLE static void error   (const QString &str);
        Q_INVOKABLE static void warning (const QString &str);
        Q_INVOKABLE static void fatal   (const QString &str);

        Q_INVOKABLE void aleus    (const QString &str);
        Q_INVOKABLE void vonabirg (const QString &str);
        Q_INVOKABLE void ded      (const QString &str);

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
} // namespace dxs {
