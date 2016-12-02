#include "DeviceInfo.h"
#include <LogSp.h>
#include <SpApplicationPrototype.h>
#include <QScreen>

#if defined(Q_OS_ANDROID)
    #include <QAndroidJniObject>
    #include <QtAndroidExtras>
    #include <QtAndroid>
#endif

using namespace sp;
//------------------------------------------------------------------------------
DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent)
{ }

//------------------------------------------------------------------------------
QString DeviceInfo::deviceID()
{
    QString token = "";
    //#if defined(Q_OS_ANDROID)
    //   token = QAndroidJniObject::callStaticObjectMethod(QString("sp/SpActivity")
    //                                                     , "getDeviceID"
    //                                                     , "(Landroid/content/Context;)Ljava/lang/String;"
    //                                                     , QtAndroid::androidActivity().object<jobject>()).toString();
    //#endif

    return token;
    //return QString(QCryptographicHash::hash(  token.toUtf8(), QCryptographicHash::Sha256   ).toHex() );
}

//------------------------------------------------------------------------------
float DeviceInfo::getAndroidVersion()
{
    #ifdef Q_OS_IOS
        return getIosVersion();
    #elif defined(Q_OS_ANDROID)
    //    int version = (int)QAndroidJniObject::QAndroidJniObject::callStaticMethod<jint>(
    //                                                        QString("sp/SpActivity")
    //                                                        ,"getVersion"
    //                                                        , "()I" );
    //    return version;
    #endif
    return 0;
}

//------------------------------------------------------------------------------
int DeviceInfo::availableRam()
{
    //#if defined(Q_OS_ANDROID)
    //    return static_cast<int>
    //            (QAndroidJniObject::callStaticMethod<jint>(
    //                QString("sp/SpActivity")
    //                , "availableRAM"
    //                , "(Landroid/content/Context;)J"
    //                , QtAndroid::androidActivity().object<jobject>()));
    //#endif

    return 0;
}

//------------------------------------------------------------------------------
void DeviceInfo::killApp()
{
    LOG_INFO("Завершаем приложение, с кодом 0");
    #ifdef Q_OS_ANDROID
        QtAndroid::androidActivity().callMethod<void>("quitApplication");
    #else
        qApp->exit();
    #endif
}

//------------------------------------------------------------------------------
int DeviceInfo::statusBarHeight()
{
    #if defined (Q_OS_ANDROID)
        QAndroidJniObject activity = QtAndroid::androidActivity();
        jint height = activity.callMethod<jint>("statusBarHeight");

        return (int) height;
    #elif defined (Q_OS_IOS)
        //TODO Добавить код для iOS
        return 0;
    #endif

    #if defined (QT_NO_DEBUG)
        return 0;
    #else
        return 2.5*qApp->mm();
    #endif
}

//------------------------------------------------------------------------------
bool DeviceInfo::abilityChangeStatusBar()
{
    #if defined (Q_OS_ANDROID)
        //Все, что выше KitKat - подходит.
        QAndroidJniObject activity = QtAndroid::androidActivity();
        jboolean j_availible = activity.callMethod<jboolean>("availibleChangeStatusBar");
        return j_availible == JNI_TRUE;
    #endif

    return true;
}

//------------------------------------------------------------------------------
DeviceInfo &DeviceInfo::instance(QObject *parent)
{
    static DeviceInfo instance(parent);
    return instance;
}
