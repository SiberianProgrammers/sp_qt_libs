#pragma once

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT

    public:
        DeviceInfo(QObject *parent = NULL);
        static DeviceInfo &instance(QObject *parent = 0);

        Q_INVOKABLE int statusBarHeight();

    private:
        static DeviceInfo _instance;
};
