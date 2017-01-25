import QtQuick 2.7

Item {
    id: window

    height: Window.height
    width : Window.width

    Rectangle {
        anchors.fill: parent
        color: "#78a3b6"
    }

    Image {
        source: "qrc:/images/habralogo.png"
        width: 0.8*parent.width
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
    }
}
