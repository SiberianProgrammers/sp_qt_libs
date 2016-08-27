import QtQuick 2.5
import QtQuick.Window 2.2
import DXS 1.0

Window {
    readonly property double mm: Screen.pixelDensity
    readonly property double margin: 2*mm

    visible: true
    width: 400
    height: 450
    title: qsTr("Hello World")

    //ListView {
    //    id: listView

    //    model: 100
    //    delegate: ContactDelegate {}
    //    anchors.fill: parent
    //    maximumFlickVelocity: 8*height
    //}

    BenchmarkItem {
        id: benchmarkItem
    }

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
