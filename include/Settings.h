#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

namespace sp {
/***************************************************************************//**
 * @brief Обёртка для просто использования настроек в C++ и QML.
 *
 * @details
 ******************************************************************************/
class Settings : public QObject
{
    Q_OBJECT

    public:
        static Settings& instance();

        /**
         * @brief Устанавливет значение переменной в настройках.
         *
         * @param name название переменной
         * @param value устанавливаемое значение
         */
        Q_INVOKABLE static
        void set(const QString &key, const QVariant &value);

        /**
         * @brief Возвращает значение переменной из настроек.
         *
         * @param name название
         * @param defaultValue значение по умолчанию, возвращаемое, если переменной нет в настройках
         */
        Q_INVOKABLE static
        QVariant get(const QString &key, const QVariant &defaultValue = QVariant());

    private:
        explicit Settings();
        ~Settings();

        void onSet(const QString &key, const QVariant &value);
        QVariant onGet(const QString &key, const QVariant &defaultValue) const;

    private:
        QSettings _settings;
};
} // namespace sp

#endif // SETTINGS_H
