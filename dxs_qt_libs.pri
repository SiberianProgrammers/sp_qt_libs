INCLUDEPATH += $$PWD/include
HEADERS += \
    $$PWD/include/NetworkAccessManagerWorker.h \
    $$PWD/include/FileLoader.h \
    $$PWD/include/Settings.h

SOURCES += \
    $$PWD/source/NetworkAccessManagerWorker.cpp \
    $$PWD/source/FileLoader.cpp \
    $$PWD/source/Settings.cpp

# Внешние ресурсы
RESOURCES += \
    $$PWD/DxsQml/DxsQml.qrc \
