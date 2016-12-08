import QtQuick 2.5
import SP 1.0

Item {
    id: root

    readonly property real margin: 3*mm

    visible: true
    width: Window.width
    height: Window.height

    //ListView {
    //    id: listView

    //    model: 400
    //    delegate: ContactDelegate {}
    //    anchors.fill: parent
    //    maximumFlickVelocity: 8*height
    //}

    //Rectangle {
    //    id: containerBackground

    //    anchors.fill: container
    //    color: "white"
    //    opacity: 0.0
    //}

    //Item {
    //    id: container

    //    anchors.fill: parent

    //}

    //Item {
    //    id: viaElement
    //}

    ImageFast {
        source: "qrc:/avatar.jpg"
        fillMode: ImageFast.PreserveAspectFit
        verticalAlignment: ImageFast.AlignTop
        horizontalAlignment: ImageFast.AlignRight
        radius: 5*mm
        anchors {
            fill: parent
            margins: margin
        }
    }
}
