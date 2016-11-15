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
    $$PWD/Include/ImageSpLoader.h

SOURCES += \
    $$PWD/Source/NetworkAccessManagerWorker.cpp \
    $$PWD/Source/FileLoader.cpp \
    $$PWD/Source/Settings.cpp \
    $$PWD/Source/Arc.cpp \
    $$PWD/Source/ArcFast.cpp \
    $$PWD/Source/SpApplicationPrototype.cpp \
    $$PWD/Source/LogSp.cpp \
    $$PWD/Source/ImageSp.cpp \
    $$PWD/Source/ImageSpLoader.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/SpQml/SpQml.qrc \

android {
    DISTFILES += \
        $$PWD/android/src/sp/SpActivity.java
}
