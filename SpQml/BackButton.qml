import QtQuick 2.8

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Кнопка назад
//------------------------------------------------------------------------------
MaterialButton {
    id: backButton

    property string arrowColor: "white"
    property color  borderColor: "transparent"
    property real   borderWidth: 0.1

    color: "transparent"
    border.width: 0
    width: height
    radius: height/2

    Rectangle {
        id: rectangleTop

        color: backButton.arrowColor
        height: Consts.lineWidth
        width: 2.5*mm
        anchors {
            top: rectangleMiddle.verticalCenter
            left: rectangleMiddle.left
            topMargin: -0.15*height
            leftMargin: -height
        }
        rotation: -45
        transformOrigin: Item.TopLeft

        border {
            width: borderWidth
            color: borderColor
        }
    }

    Rectangle {
        id: rectangleMiddle

        color: backButton.arrowColor
        height: Consts.lineWidth
        width: 3*mm
        anchors.centerIn: parent
        border {
            width: borderWidth
            color: borderColor
        }
    }

    Rectangle {
        id: rectangleBottom

        color: backButton.arrowColor
        height: Consts.lineWidth
        width: 2.5*mm
        anchors {
            left: rectangleMiddle.left
            bottom: rectangleMiddle.verticalCenter
            leftMargin: -height
            bottomMargin: 0.15*height
        }
        rotation: 45
        transformOrigin: Item.BottomLeft
        border {
            width: borderWidth
            color: borderColor
        }
    }
}
