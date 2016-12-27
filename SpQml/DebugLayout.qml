import QtQuick 2.7

//--------------------------------------------------------------------------
// Крест, показывающий центр родителя
//--------------------------------------------------------------------------
Item {
    id: _debugLayout

    property color color: "red"
    anchors.fill: parent
    z: 1000

    //--------------------------------------------------------------------------
    Rectangle {
        width: 1
        height: parent.height
        color: _debugLayout.color
        anchors.centerIn: parent
    }

    //--------------------------------------------------------------------------
    Rectangle {
        width: parent.width
        height: 1
        color: _debugLayout.color
        anchors.centerIn: parent
    }
}
