#include <QDebug>
#include "Settings.h"
#include "SpApplicationPrototype.h"

using namespace sp;

Settings::Settings(const QString &applicationName, QObject *parent)
    : QObject(parent)
    , _settings(QSettings::IniFormat, QSettings::UserScope, "SP", applicationName)
{
}

Settings &Settings::instance()
{
    static Settings instance_(qApp->applicationName(), nullptr);
    return instance_;
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
    if (defaultValue.type() < QVariant::UserType) {
        v.convert(defaultValue.type());
    }

    return v;
}

