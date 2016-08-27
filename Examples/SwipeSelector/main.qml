import QtQuick 2.5
import QtQuick.Window 2.2
import "qrc:/DxsQml"

Window {
    visible: true
    width: 200
    height: 350

    property bool blueOnTop: true
    property SwipeSelector swipeSelectorTop: blueOnTop
                                             ? blue.swipeSelector
                                             : green.swipeSelector
    property real mm: Screen.pixelDensity
    property real margin: 4*mm

    Item {
        anchors.fill: parent

        Card {
            id: blue

            onTop: blueOnTop
        }

        Card {
            id: green

            color: "green"
            onTop: !blueOnTop
        }
    }
}
