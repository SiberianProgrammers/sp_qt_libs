import QtQuick 2.5
import DXS 1.0

Item {
    readonly property double margin: 2*mm

    visible: true
    width: Window.width
    height: Window.height

    ListView {
        id: listView

        model: 400
        delegate: ContactDelegate {}
        anchors.fill: parent
        maximumFlickVelocity: 8*height
    }

    //BenchmarkItem {
    //    id: benchmarkItem
    //}

    //--------------------------------------------------------------------------
    //ImageDxs {
    //    id: image

    //    source: "qrc:/avatar.jpg"
    //    anchors.centerIn: parent
    //    fillMode: ImageDxs.PreserveAspectCrop

    //    width: parent.width - 2*margin
    //    height: width
    //    blur: true
    //    radius: width/2
    //}

    //MouseArea {
    //    anchors.fill: image
    //    onClicked: {
    //        image.blur = !image.blur;
    //    }
    //}
}
