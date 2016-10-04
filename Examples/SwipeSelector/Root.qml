import QtQuick 2.5
import DXS 1.0
import "qrc:/DxsQml"

Item {
    width: Window.width
    height: Window.height

    property bool blueOnTop: true
    property SwipeSelector swipeSelectorTop: blueOnTop
                                             ? blue.swipeSelector
                                             : green.swipeSelector
    property real margin: 4*mm

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
