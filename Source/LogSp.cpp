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
}

//------------------------------------------------------------------------------
Log &Log::instance()
{
    return _instance;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

//------------------------------------------------------------------------------
void Log::debug(const QString &str)
{
    QString formatedStr = QString::number(_time.elapsed()) % ": " % str;

    #ifdef Q_OS_ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "#######", "%s", formatedStr.toLocal8Bit().constData());
    #else
        qDebug(formatedStr.toUtf8());
    #endif
    _instance.toArchive(formatedStr);
}

//------------------------------------------------------------------------------
void Log::info(const QString &str)
{
    QString formatedStr = QString::number(_time.elapsed()) % ": " % str;

    #ifdef Q_OS_ANDROID
        __android_log_print(ANDROID_LOG_INFO, "#######", "%s", formatedStr.toLocal8Bit().constData());
    #else
        qDebug(formatedStr.toUtf8());
    #endif
    _instance.toArchive(formatedStr);
}

//--------------------------------------------------------------------------
void Log::error(const QString &str)
{
    QString formatedStr = QString::number(_time.elapsed()) % ": " % str;

    #ifdef Q_OS_ANDROID
        __android_log_print(ANDROID_LOG_ERROR, "#######", "%s", formatedStr.toLocal8Bit().constData());
    #else
        qDebug(formatedStr.toUtf8());
    #endif
    _instance.toArchive(formatedStr);
}

//------------------------------------------------------------------------------
void Log::warning(const QString &str)
{
    QString formatedStr = QString::number(_time.elapsed()) % ": " % str;

    #ifdef Q_OS_ANDROID
        __android_log_print(ANDROID_LOG_WARN, "#######", "%s", formatedStr.toLocal8Bit().constData());
    #else
        qDebug(formatedStr.toUtf8());
    #endif
    _instance.toArchive(formatedStr);
}

//--------------------------------------------------------------------------
void Log::fatal(const QString &str)
{
    QString formatedStr = QString::number(_time.elapsed()) % ": " % str;

    #ifdef Q_OS_ANDROID
        __android_log_print(ANDROID_LOG_FATAL, "#######", "%s", formatedStr.toLocal8Bit().constData());
    #else
        qDebug(formatedStr.toUtf8());
    #endif
    _instance.toArchive(formatedStr);
}

//--------------------------------------------------------------------------
void Log::aleus(const QString &str) {
#ifdef SP_ALEUS
    LOG_ALEUS(str);
#else
    Q_UNUSED(str);
#endif
}

//--------------------------------------------------------------------------
void Log::vonabirg(const QString &str) {
#ifdef SP_VONABIRG
    LOG_VONABIRG(str);
#else
    Q_UNUSED(str);
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

#pragma GCC diagnostic pop
