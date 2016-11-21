#include <QApplication>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroidExtras>
#endif

#include "KeyboardSp.h"

//------------------------------------------------------------------------------
sp::KeyboardSp &sp::KeyboardSp::instance()
{
    static KeyboardSp instance;
    return instance;
}

//------------------------------------------------------------------------------
void sp::KeyboardSp::sendVisibleChanged(bool visible, int height)
{
    KeyboardSp &instance = KeyboardSp::instance();

    if (qApp) {
        if (instance._height != height) {
            instance._height = height;
            emit instance.heightChanged(instance._height);
        }

        if (instance._visible != visible || instance._height != height) {
            instance._visible = visible;
            emit instance.visibleChanged(instance._visible);
        }
    }
}

//------------------------------------------------------------------------------
sp::KeyboardSp::KeyboardSp()
    : QObject()
{

}

//------------------------------------------------------------------------------
void sp::KeyboardSp::show() {
    #ifdef Q_OS_ANDROID
        QAndroidJniObject activity = QtAndroid::androidActivity();
        activity.callMethod<void>("showKeyboard");
    #endif
}

//------------------------------------------------------------------------------
void sp::KeyboardSp::hide() {
    #ifdef Q_OS_ANDROID
        QAndroidJniObject activity = QtAndroid::androidActivity();
        activity.callMethod<void>("hideKeyboard");
    #endif
}
