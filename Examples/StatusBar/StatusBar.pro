TEMPLATE = app

QT += qml quick
CONFIG += c++11

INCLUDEPATH += Include

HEADERS += \
    Include/DeviceInfo.h

SOURCES += \
    Source/main.cpp

RESOURCES += \
    QML/qml.qrc \
    Images/Images.qrc

android {
    QT += androidextras

    DISTFILES += \
        Android/AndroidManifest.xml \
        Android/gradle/wrapper/gradle-wrapper.jar \
        Android/gradlew \
        Android/res/values/libs.xml \
        Android/build.gradle \
        Android/gradle/wrapper/gradle-wrapper.properties \
        Android/gradlew.bat \
        Android/src/com/sp/statusBar/StatusBarActivity.java

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/Android
}

!ios {
    SOURCES += \
        Source/DeviceInfo.cpp
}

ios {
    OBJECTIVE_SOURCES += \
        Source/DeviceInfo.mm

    QMAKE_INFO_PLIST = iOS/Info.plist
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


