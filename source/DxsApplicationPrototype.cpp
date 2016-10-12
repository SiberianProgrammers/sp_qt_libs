#include "DxsApplicationPrototype.h"

#include "Arc.h"
#include "ArcFast.h"
#include "ImageDxs.h"

//--------------------------------------------------------------------------
dxs::DxsApplicationPrototype::DxsApplicationPrototype(int &argc, char **argv, const QString &title, int width, int height)
    : QGuiApplication (argc, argv)
    , _view ()
{
    QObject::connect(_view.engine(), SIGNAL(quit()), this, SLOT(quit()));

    _view.setTitle(title);
    _view.setColor(Qt::white);
    #ifdef Q_OS_OSX
        _view.setWidth(width);
        _view.setHeight(height);
    #else
        _view.setWidth(primaryScreen()->size().width());
        _view.setHeight(primaryScreen()->size().height());
    #endif

    // Включаем антиалиасинг
    QSurfaceFormat format = _view.format();
    format.setSamples(16);
    _view.setFormat(format);

    // Регистрируем C++ класса в QML
    qmlRegisterType<dxs::Arc>("DXS", 1, 0, "Arc");
    qmlRegisterType<dxs::ArcFast>("DXS", 1, 0, "ArcFast");
    qmlRegisterType<dxs::ImageDxs>("DXS", 1, 0, "ImageDxs");

    // Создаём объекты QML
    const double mmInInch = 25.4;
    _view.rootContext()->setContextProperty("mm", screens().first()->physicalDotsPerInch()/mmInInch);
    _view.rootContext()->setContextProperty("Window", &_view);
}

//--------------------------------------------------------------------------
int dxs::DxsApplicationPrototype::exec(const QUrl &source)
{
    _view.setSource(source); // по умолчанию "qrc:/Root.qml"

    // Открываем основной файл
    #ifdef Q_OS_IOS
        _view.showFullscreen();
    #else
        _view.show();
    #endif
    return QGuiApplication::exec();
}
