import QtQuick 2.5
import SP 1.0

Item {
    readonly property double margin: 2*mm
    property bool fast: false

    width: Window.width
    height: Window.height

    //--------------------------------------------------------------------------
    Loader {
        id: arcLoader

        sourceComponent: fast
                         ? arcFastComponent
                         : arcComponent
        anchors {
            fill: parent
            margins: margin
        }
    }

    //--------------------------------------------------------------------------
    MouseArea {
        anchors.fill: parent
        onClicked: {
            animation.stop();
            fast = !fast;
        }

        Column {
            anchors.centerIn: parent
            spacing: margin

            Text {
                id: text
                text: fast
                      ? "ArcFast"
                      : "Arc"
                font.pointSize: 14
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: "Нажми, чтобы\nсменить компонент."
                color: "darkgray"
                font.pointSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    //--------------------------------------------------------------------------
    // Дуга с отрисовкой в QPainter
    //--------------------------------------------------------------------------
    Component {
        id: arcComponent

        Arc {
            spanAngle: 0
            penWidth: 0.25*mm
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
    SequentialAnimation {
        id: animation

        running: arcLoader.status === Loader.Ready
        loops: Animation.Infinite

        NumberAnimation {
            target: arcLoader.item
            property: "spanAngle"
            from: 0
            to: 360
            duration: 1000
        }
        ParallelAnimation {
            NumberAnimation {
                target: arcLoader.item
                property: "spanAngle"
                from: 360
                to: 0
                duration: 1000
            }
            NumberAnimation {
                target: arcLoader.item
                property: "startAngle"
                from: 0
                to: 360
                duration: 1000
            }
        }
    }
}
