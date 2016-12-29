QT += widgets
INCLUDEPATH += $$PWD/Include
HEADERS += \
    $$PWD/Include/NetworkAccessManagerWorker.h \
    $$PWD/Include/FileLoader.h \
    $$PWD/Include/Settings.h \
    $$PWD/Include/Arc.h \
    $$PWD/Include/ArcFast.h \
    $$PWD/Include/SpApplicationPrototype.h \
    $$PWD/Include/LogSp.h \
    $$PWD/Include/ImageSp.h \
    $$PWD/Include/Priavte/ImageSpNode.h \
    $$PWD/Include/Private/ImageSpLoader.h \
    $$PWD/Include/ImageSaverSp.h \
    $$PWD/Include/ImageParallax.h \
    $$PWD/Include/DeviceInfo.h \
    $$PWD/Include/KeyboardSp.h \
    $$PWD/Include/Arrow.h \
    $$PWD/Include/Shadow.h \
    $$PWD/Include/Net.h \
    $$PWD/Include/Private/DownloadFileHandler.h \
    $$PWD/Include/Private/NetHandler.h

SOURCES += \
    $$PWD/Source/NetworkAccessManagerWorker.cpp \
    $$PWD/Source/FileLoader.cpp \
    $$PWD/Source/Settings.cpp \
    $$PWD/Source/Arc.cpp \
    $$PWD/Source/ArcFast.cpp \
    $$PWD/Source/LogSp.cpp \
    $$PWD/Source/ImageSp.cpp \
    $$PWD/Source/Private/ImageSpNode.cpp \
    $$PWD/Source/Private/ImageSpLoader.cpp \
    $$PWD/Source/ImageSaverSp.cpp \
    $$PWD/Source/ImageParallax.cpp \
    $$PWD/Source/DeviceInfo.cpp \
    $$PWD/Source/Arrow.cpp \
    $$PWD/Source/Shadow.cpp \
    $$PWD/Source/Net.cpp \
    $$PWD/Source/Private/DownloadFileHandler.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/SpQml/SpQml.qrc \

android {
    DISTFILES += \
        $$PWD/android/src/sp/SpActivity.java
}

!mac {
    SOURCES += \
        $$PWD/Source/SpApplicationPrototype.cpp \
        $$PWD/Source/KeyboardSp.cpp
}

mac {
    OBJECTIVE_SOURCES += \
        $$PWD/Source/SpApplicationPrototype.mm \
        $$PWD/Source/KeyboardSp.mm
}


