/// @author M. Serebrennikov
#include "Settings.h"
#include "SpApplicationPrototype.h"

#include <QVariantMap>

using namespace sp;

Settings &Settings::instance()
{
    static Settings instance;
    return instance;
}

//------------------------------------------------------------------------------
Settings::Settings()
    : _settings(QSettings::IniFormat, QSettings::UserScope, "SP", qApp->applicationName())
{
}

//------------------------------------------------------------------------------
Settings::~Settings()
{
    _settings.sync();
}

//------------------------------------------------------------------------------
void Settings::onSet(const QString &key, const QVariant &value)
{
    _settings.setValue(key, value);
    _settings.sync();
}

//------------------------------------------------------------------------------
QVariant Settings::onGet(const QString &key, const QVariant &defaultValue) const
{
    if (_settings.contains(key)) {
        QVariant v = _settings.value(key);

        if (v.canConvert<QVariantMap>()) {
            return v;
        } else {
            v.convert(defaultValue.type());
            return v;
        }
    } else {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
void Settings::set(const QString &key, const QVariant &value)
{
    return instance().onSet(key, value);
}

//------------------------------------------------------------------------------
QVariant Settings::get(const QString &key, const QVariant &defaultValue)
{
    return instance().onGet(key, defaultValue);
}
