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
            left: rectangleMiddle.left
            leftMargin: -height
            top: rectangleMiddle.verticalCenter
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
            leftMargin: -height
            bottom: rectangleMiddle.verticalCenter
        }
        rotation: 45
        transformOrigin: Item.BottomLeft
    }
}
