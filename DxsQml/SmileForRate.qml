import QtQuick 2.5
import DXS 1.0

// Оценка с помощью смайла. Для работы требуются Arc & ArcFast поключаемые с помощью
// qmlRegisterType<dxs::Arc>("DXS", 1, 0, "Arc");
// qmlRegisterType<dxs::ArcFast>("DXS", 1, 0, "ArcFast");

Item {
    id: smileForRate

    property var  evaluations: ["Ужасно","Плохо","Нормально","Хорошо","Отлично"]
    property alias backgroundColor: background.color
    property bool  useFastArc: false
    property real  penWidth: mm
    property real  currentRate: Math.round(evaluations.length * 10*(_p.dragTargetDelta + dragTarget.y) / (2*_p.dragTargetDelta)) / 10

    //--------------------------------------------------------------------------
    Rectangle {
        id: background
        color: "transparent"
        anchors.fill: parent
    }

    Text {
        id: counterItem

        text: currentRate+".0<font color=\"#979797\">/" + evaluations.length + ".0</font>"
        color: "black"
        font.pixelSize: 4*mm

        anchors {
            top: parent.top
            topMargin: margin
            horizontalCenter: parent.horizontalCenter
        }
    }

    Text {
        text: evaluations[Math.min(Math.floor(currentRate), evaluations.length - 1)]
        color: "black"
        font.pixelSize: 2.5*mm

        anchors {
            top: counterItem.bottom
            topMargin: margin
            horizontalCenter: parent.horizontalCenter
        }
    }

    //--------------------------------------------------------------------------
    Rectangle {
        id: smileCircle

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent
        width: parent.width - 2*margin - 2*penWidth
        height: width
        radius: width / 2
        color: "transparent"
        border {
            width: penWidth
            color: _p.arcColor
        }

        //--------------------------------------------------------------------------
        Loader {
            id: arcLoader

            sourceComponent: smileForRate.useFastArc
                             ? arcFastComponent
                             : arcComponent

            width: 0.5*parent.width
            height: width

            anchors.centerIn: parent
            anchors.verticalCenterOffset: 0.2*parent.height
        }

    }


    //--------------------------------------------------------------------------
    Item {
        id: dragTarget

        height: parent.height
        width: parent.width
    }

    //--------------------------------------------------------------------------
    MouseArea {
        anchors.fill: parent
        drag.target: dragTarget
        drag.axis: Drag.YAxis
        drag.minimumY: -_p.dragTargetDelta
        drag.maximumY: _p.dragTargetDelta
    }

    //--------------------------------------------------------------------------
    // Дуга с отрисовкой в QPainter
    //--------------------------------------------------------------------------
    Component {
        id: arcComponent

        Arc {
            anchors.fill: parent
            startAngle: _p.startArcAngle
            spanAngle: 150
            penWidth: smileForRate.penWidth
            arcHeight: _p.arcHeight
            color: _p.arcColor
        }

    }

    //--------------------------------------------------------------------------
    // Дуга с отрисовкой в SceneGraph
    //--------------------------------------------------------------------------
    Component {
        id: arcFastComponent

        ArcFast {
            spanAngle: 0
            penWidth: 0.25*mm
        }
    }

    //--------------------------------------------------------------------------
    QtObject {
        id: _p

        property real dragTargetDelta: 0.7*arcLoader.height
        property real startArcAngle: dragTarget.y < 0
                                        ? 15
                                        : 195

        property real arcHeight: dragTarget.y < 0
                                    ? -dragTarget.y
                                    : dragTarget.y

        property real delta: arcHeight/_p.dragTargetDelta

        property color arcColor: dragTarget.y < 0
                                  ? Qt.rgba( // От синего до красного
                                          Math.max(122/255 + 58/255*delta*4, 180/255)
                                        , Math.max(159/255 - 48/255*delta*4, 111/255)
                                        , Math.max(197/255 - 87/255*delta*4, 110/255)
                                        , 1 )
                                  : dragTarget.y === 0
                                    ? Qt.rgba(122,159,197,1)
                                    : Qt.rgba( // От синего до зеленого
                                          Math.max(122/255 - 7/255*delta *2, 115/255)
                                        , Math.max(159/255 + 35/255*delta*2, 194/255)
                                        , Math.max(197/255 - 27/255*delta*2, 170/255)
                                        , 1 )
    }
}
