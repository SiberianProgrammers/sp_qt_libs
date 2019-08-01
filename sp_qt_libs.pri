QT += widgets
INCLUDEPATH += $$PWD/include
HEADERS += \
    $$PWD/include/Settings.h \
    $$PWD/include/Arc.h \
    $$PWD/include/ArcFast.h \
    $$PWD/include/SpApplicationPrototype.h \
    $$PWD/include/LogSp.h \
    $$PWD/include/ImageSp.h \
    $$PWD/include/Priavte/ImageSpNode.h \
    $$PWD/include/Private/ImageSpLoader.h \
    $$PWD/include/ImageSaverSp.h \
    $$PWD/include/ImageParallax.h \
    $$PWD/include/DeviceInfo.h \
    $$PWD/include/KeyboardSp.h \
    $$PWD/include/Arrow.h \
    $$PWD/include/Shadow.h \
    $$PWD/include/Net.h \
    $$PWD/include/DownloadFileHandler.h \
    $$PWD/include/Private/NetHandler.h

SOURCES += \
    $$PWD/source/Settings.cpp \
    $$PWD/source/Arc.cpp \
    $$PWD/source/ArcFast.cpp \
    $$PWD/source/LogSp.cpp \
    $$PWD/source/ImageSp.cpp \
    $$PWD/source/Private/ImageSpNode.cpp \
    $$PWD/source/Private/ImageSpLoader.cpp \
    $$PWD/source/ImageSaverSp.cpp \
    $$PWD/source/ImageParallax.cpp \
    $$PWD/source/DeviceInfo.cpp \
    $$PWD/source/Arrow.cpp \
    $$PWD/source/Shadow.cpp \
    $$PWD/source/Net.cpp \
    $$PWD/source/DownloadFileHandler.cpp \
    $$PWD/source/Private/NetHandler.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/qml/qml.qrc \

android {
    QT += androidextras

    DISTFILES += \
        $$PWD/android/src/sp/SpActivity.java
}

!mac {
    SOURCES += \
        $$PWD/source/SpApplicationPrototype.cpp \
        $$PWD/source/KeyboardSp.cpp
}

mac {
    OBJECTIVE_SOURCES += \
        $$PWD/source/SpApplicationPrototype.mm \
        $$PWD/source/KeyboardSp.mm
}


