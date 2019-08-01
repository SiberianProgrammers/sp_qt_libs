#include "DeviceInfo.h"

#import <UIKit/UIKit.h>

DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent)
{
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
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
    return [UIApplication sharedApplication].statusBarFrame.size.height;
}


