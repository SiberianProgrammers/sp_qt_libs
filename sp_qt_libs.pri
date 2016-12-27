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
    $$PWD/Include/ImageSpLoader.h \
    $$PWD/Include/DeviceInfo.h \
    $$PWD/Include/ImageParallax.h \
    $$PWD/Include/KeyboardSp.h \
    $$PWD/Include/Arrow.h \
    $$PWD/Include/Shadow.h \
    $$PWD/Include/ImageSaverSp.h \
    $$PWD/Include/ImageSpNode.h

SOURCES += \
    $$PWD/Source/NetworkAccessManagerWorker.cpp \
    $$PWD/Source/FileLoader.cpp \
    $$PWD/Source/Settings.cpp \
    $$PWD/Source/Arc.cpp \
    $$PWD/Source/ArcFast.cpp \
    $$PWD/Source/LogSp.cpp \
    $$PWD/Source/ImageSp.cpp \
    $$PWD/Source/ImageSpLoader.cpp \
    $$PWD/Source/DeviceInfo.cpp \
    $$PWD/Source/ImageParallax.cpp \
    $$PWD/Source/Arrow.cpp \
    $$PWD/Source/Shadow.cpp \
    $$PWD/Source/ImageSaverSp.cpp \
    $$PWD/Source/ImageSpNode.cpp

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


