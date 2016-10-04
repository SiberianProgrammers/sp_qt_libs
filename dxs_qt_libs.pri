QT += widgets
INCLUDEPATH += $$PWD/include
HEADERS += \
    $$PWD/include/NetworkAccessManagerWorker.h \
    $$PWD/include/FileLoader.h \
    $$PWD/include/ImageDxs.h \
    $$PWD/include/Settings.h \
    $$PWD/include/ImageDxsLoader.h \
    $$PWD/include/Arc.h \
    $$PWD/include/ArcFast.h \
    $$PWD/include/DxsApplication.h

SOURCES += \
    $$PWD/source/NetworkAccessManagerWorker.cpp \
    $$PWD/source/FileLoader.cpp \
    $$PWD/source/Settings.cpp \
    $$PWD/source/ImageDxs.cpp \
    $$PWD/source/ImageDxsLoader.cpp \
    $$PWD/source/Arc.cpp \
    $$PWD/source/ArcFast.cpp \
    $$PWD/source/DxsApplication.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/DxsQml/DxsQml.qrc \
