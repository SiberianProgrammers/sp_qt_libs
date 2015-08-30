import QtQuick 2.5

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Кнопка открытия меню с иконкой в виде бутерброда.
//------------------------------------------------------------------------------
MaterialButton {
    id: menuButton

    property string color: "white"

    activeColor: "transparent"
    width: height

    Column {
        anchors.centerIn: parent
        spacing: Math.ceil(0.5*Consts.mm)

        Rectangle {
            id: rectangleTop

            color: menuButton.color
            height: Math.ceil(0.5*Consts.mm)
            width: 4*Consts.mm
        }

        Rectangle {
            id: rectangleMiddle

            color: menuButton.color
            height: Math.ceil(0.5*Consts.mm)
            width: 4*Consts.mm
        }

        Rectangle {
            id: rectangleBottom

            color: menuButton.color
            height: Math.ceil(0.5*Consts.mm)
            width: 4*Consts.mm
        }
    }
}
