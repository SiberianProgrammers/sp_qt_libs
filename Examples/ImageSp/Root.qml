import QtQuick 2.5
import SP 1.0

Item {
    id: root

    readonly property real margin: 3*mm

    visible: true
    width: Window.width
    height: Window.height

    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    ImageSp {
        property bool avatarSwitch: true

        source: avatarSwitch
                ? "qrc:/avatar.jpg"
                : "qrc:/avatar2.jpg"
        //fillMode: ImageSp.PreserveAspectFit
        radius: 5*mm
        anchors {
            fill: parent
            margins: margin
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.avatarSwitch = !parent.avatarSwitch;
                //parent.mipmap = !parent.mipmap;
            }
        }
    }// ImageFast {
}
