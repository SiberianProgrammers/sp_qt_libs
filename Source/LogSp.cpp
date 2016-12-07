#include <QDebug>
#include <QStringBuilder>
#ifdef Q_OS_ANDROID
#include <QCoreApplication>
#include <android/log.h>
#endif

#include "LogSp.h"

using namespace sp;

Log         Log::_instance;
QTime       Log::_time;
QStringList Log::_logArchive;

//------------------------------------------------------------------------------
Log::Log(QObject *parent)
    :QObject (parent)
{
    _time.start();
    qInstallMessageHandler(sp::Log::messageHandler);
}

//------------------------------------------------------------------------------
Log &Log::instance()
{
    return _instance;
}

void Log::debug(const QString &str)
{
    LOG_DEBUG(str);
}

void Log::info(const QString &str)
{
    LOG_INFO(str);
}

void Log::error(const QString &str)
{
    LOG_ERROR(str);
}

void Log::warning(const QString &str)
{
    LOG_WARN(str);
}

void Log::fatal(const QString &str)
{
    LOG_FATAL(str);
}

void Log::aleus(const QString &str)
{
    #ifdef SP_ALEUS
        LOG_ALEUS(str);
    #endif
}

void Log::vonabirg(const QString &str)
{
    #ifdef SP_VONABIRG
        LOG_V(str);
    #endif
}

//--------------------------------------------------------------------------
QString Log::logArchive() const  {
    return _logArchive.join('\n');
}

//------------------------------------------------------------------------------
void Log::toArchive(const QString &str)
{
    _logArchive.append(str);
}

//------------------------------------------------------------------------------
void Log::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QString formatedStr =
        #ifdef QT_MESSAGELOGCONTEXT
            type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg
            ? QString::number(_time.elapsed()) + ": [" % QString(context.file) % ":" % QString::number(context.line) % ", " % context.function % "] " % message % '\n'
            :
        #endif
            QString::number(_time.elapsed()) % ": " % message % '\n';

    #ifdef Q_OS_ANDROID
        android_LogPriority priority = ANDROID_LOG_INFO;
        switch (type) {
            case QtWarningMsg:
                priority = ANDROID_LOG_WARN;
                break;
            case QtCriticalMsg:
                priority = ANDROID_LOG_ERROR;
                break;
            case QtFatalMsg:
                priority = ANDROID_LOG_FATAL;
                break;

        };
        __android_log_print(priority, "#######", "%s", formatedStr.toLocal8Bit().constData());

    #else
        QByteArray localMsg = message.toLocal8Bit();
        fprintf(stderr, formatedStr.toUtf8().constData());
    #endif

    _instance.toArchive(formatedStr);
}

#pragma GCC diagnostic pop
