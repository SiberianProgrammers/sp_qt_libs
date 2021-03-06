#include "SpApplicationPrototype.h"

#include "Arc.h"
#include "ArcFast.h"
#include "Arrow.h"
#include <QDebug>
#include "ImageSp.h"
#include "LogSp.h"
#include "DeviceInfo.h"
#include "ImageParallax.h"
#include "KeyboardSp.h"
#include "Shadow.h"
#include "ImageSaverSp.h"
#include "Settings.h"

#if defined(Q_OS_ANDROID)
#include "JniSetup.h"
#elif defined(Q_OS_IOS)
#import <UIKit/UIKit.h>
#endif

using namespace sp;

//--------------------------------------------------------------------------
sp::SpApplicationPrototype::SpApplicationPrototype(int &argc, char **argv, const QString &title, int width, int height)
    : QGuiApplication (argc, argv)
    , _view ()
{
    QObject::connect(_view.engine(), SIGNAL(quit()), this, SLOT(quit()));
    _view.setTitle(title);
    _view.setColor(Qt::white);

    #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
        _view.setWidth(primaryScreen()->size().width());
        _view.setHeight(primaryScreen()->size().height());
    #else
        _view.setWidth(width);
        _view.setHeight(height);
    #endif

    // Включаем антиалиасинг.
    #if !defined(Q_OS_LINUX) // Под линуксом на старющей видеокарте не работает setSamples 16.
        QSurfaceFormat format = _view.format();
        format.setSamples(16);
        _view.setFormat(format);
    #else
        QSurfaceFormat format = _view.format();
        format.setSamples(8);
        _view.setFormat(format);
    #endif

    // Регистрируем C++ класса в QML
    qmlRegisterType<sp::Arc>("SP", 1, 0, "Arc");
    qmlRegisterType<sp::ArcFast>("SP", 1, 0, "ArcFast");
    qmlRegisterType<sp::Arrow>("SP", 1, 0, "Arrow");
    qmlRegisterType<sp::ImageSp>("SP", 1, 0, "ImageSp");
    qmlRegisterType<sp::ImageParallax>("SP", 1, 0, "ImageParallax");
    qmlRegisterType<sp::Shadow>("SP", 1, 0, "Shadow");

    // Создаём объекты QML
    const double mmInInch = 25.4;
    _mm = screens().first()->physicalDotsPerInch()/mmInInch;

    _view.rootContext()->setContextProperty("mm", _mm);
    _view.rootContext()->setContextProperty("Window", &_view);
    _view.rootContext()->setContextProperty("imageSaverSp", new ImageSaverSp());
    _view.rootContext()->setContextProperty("Log", &sp::Log::instance());
    _view.rootContext()->setContextProperty("deviceInfo", &sp::DeviceInfo::instance());
    _view.rootContext()->setContextProperty("KeyboardSp", &sp::KeyboardSp::instance());
    _view.rootContext()->setContextProperty("Settings", &sp::Settings::instance());

    #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
        _view.rootContext()->setContextProperty("isDesktop", false);
    #else
        _view.rootContext()->setContextProperty("isDesktop", true);
    #endif

}

//--------------------------------------------------------------------------
int sp::SpApplicationPrototype::exec(const QUrl &source)
{
    _view.setSource(source); // по умолчанию "qrc:/Root.qml"

    // Открываем основной файл
    #ifdef Q_OS_IOS
        _view.showFullScreen();
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    #else
        _view.show();
    #endif

    return QGuiApplication::exec();
}
