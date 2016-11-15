#include <QDebug>
#include "Settings.h"

using namespace sp;

Settings::Settings(const QString &applicationName, QObject *parent)
    : QObject(parent)
    , _settings(QSettings::IniFormat, QSettings::UserScope, "SP", applicationName)
{
    //qDebug() << "settings file path" << _settings.fileName();
}

Settings::~Settings()
{

}

void Settings::set(const QString &key, const QVariant &value)
{
    _settings.setValue(key, value);
}

QVariant Settings::get(const QString &key, const QVariant &defaultValue /* = QVariant() */) const
{
    QVariant v = _settings.value(key, defaultValue);
    v.convert(defaultValue.type());

    return v;
}

