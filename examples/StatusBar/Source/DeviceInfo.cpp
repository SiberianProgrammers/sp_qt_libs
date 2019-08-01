#include "DeviceInfo.h"

#if defined(Q_OS_ANDROID)
    #include <QAndroidJniObject>
    #include <QtAndroidExtras>
    #include <QtAndroid>
#endif

DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent)
{

}

//------------------------------------------------------------------------------
DeviceInfo &DeviceInfo::instance(QObject *parent)
{
    static DeviceInfo instance(parent);
    return instance;
}

//------------------------------------------------------------------------------
int DeviceInfo::statusBarHeight()
{
#if defined (Q_OS_ANDROID)
    QAndroidJniObject activity = QtAndroid::androidActivity();
    jint height = activity.callMethod<jint>("statusBarHeight");

    return (int) height;
#endif
    return 0;
}


