QT += widgets
INCLUDEPATH += $$PWD/Include
HEADERS += \
    $$PWD/Include/NetworkAccessManagerWorker.h \
    $$PWD/Include/FileLoader.h \
    $$PWD/Include/ImageDxs.h \
    $$PWD/Include/Settings.h \
    $$PWD/Include/ImageDxsLoader.h \
    $$PWD/Include/Arc.h \
    $$PWD/Include/ArcFast.h \
    $$PWD/Include/DxsApplication.h \
    $$PWD/Include/LogDxs.h

SOURCES += \
    $$PWD/Source/NetworkAccessManagerWorker.cpp \
    $$PWD/Source/FileLoader.cpp \
    $$PWD/Source/Settings.cpp \
    $$PWD/Source/ImageDxs.cpp \
    $$PWD/Source/ImageDxsLoader.cpp \
    $$PWD/Source/Arc.cpp \
    $$PWD/Source/ArcFast.cpp \
    $$PWD/Source/DxsApplication.cpp \
    $$PWD/Source/LogDxs.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/DxsQml/DxsQml.qrc \
