QT += widgets
INCLUDEPATH += $$PWD/include
HEADERS += \
    $$PWD/include/NetworkAccessManagerWorker.h \
    $$PWD/include/FileLoader.h \
    $$PWD/include/ImageDxs.h \
    $$PWD/include/Settings.h

SOURCES += \
    $$PWD/source/NetworkAccessManagerWorker.cpp \
    $$PWD/source/FileLoader.cpp \
    $$PWD/source/Settings.cpp \
    $$PWD/source/ImageDxs.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/DxsQml/DxsQml.qrc \
