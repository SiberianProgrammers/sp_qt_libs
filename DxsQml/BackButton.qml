import QtQuick 2.5

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Кнопка назад
//------------------------------------------------------------------------------
MaterialButton {
    id: backButton

    property string color: "white"

    activeColor: "transparent"
    width: height

    Rectangle {
        id: rectangleTop

        color: backButton.color
        height: Consts.lineWidth
        width: 3*Consts.mm
        anchors {
            top: rectangleMiddle.verticalCenter
            left: rectangleMiddle.left
            topMargin: -0.25*height
            leftMargin: -height
        }
        rotation: -45
        transformOrigin: Item.TopLeft
    }

    Rectangle {
        id: rectangleMiddle

        color: backButton.color
        height: Consts.lineWidth
        width: 4*Consts.mm
        anchors.centerIn: parent
    }

    Rectangle {
        id: rectangleBottom

        color: backButton.color
        height: Consts.lineWidth
        width: 3*Consts.mm
        anchors {
            left: rectangleMiddle.left
            bottom: rectangleMiddle.verticalCenter
            leftMargin: -height
            bottomMargin: 0.25*height
        }
        rotation: 45
        transformOrigin: Item.BottomLeft
    }
}
