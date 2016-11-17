#pragma once

//#ifndef DEVICEINFO_H
//#define DEVICEINFO_H

#include <QObject>

namespace sp {
/**
 * @brief Класс, предоставляющий информацию об устройстве
 */
class DeviceInfo : public QObject
{
    Q_OBJECT

    public:
        DeviceInfo(QObject *parent = NULL);
        static DeviceInfo &instance(QObject *parent = 0);

        /** \brief Возвращает уникальный идентефикатор устрйоства **/
        Q_INVOKABLE QString deviceID();

        Q_INVOKABLE float getAndroidVersion();

        Q_INVOKABLE int availableRam();

        //Q_INVOKABLE int totalRam();

        Q_INVOKABLE void killApp();

        /** @brief Возвращает высоту StasusBar'a **/
        Q_INVOKABLE int getStatusBarHeight();

        /** @brief Можно ли изменять statusBar **/
        Q_INVOKABLE bool abilityChangeStatusBar();

    private:
        static DeviceInfo _instance;

};
}
//#pragma once#endif // DEVICEINFO_H
