TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

SOURCES += main.cpp 
RESOURCES += qml.qrc

include(deployment.pri)
include($$PWD/../../sp_qt_libs.pri)
