import QtQuick 2.5

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Кнопка открытия меню с иконкой в виде бутерброда.
//------------------------------------------------------------------------------
MaterialButton {
    id: menuButton

    //property string color: "white"

    color: "transparent"
    width: height


    Column {
        anchors.centerIn: parent
        spacing: Math.ceil(0.5*mm)

        Rectangle {
            id: rectangleTop

            color: menuButton.color
            height: Consts.lineWidth
            width: 4*mm
        }

        Rectangle {
            id: rectangleMiddle

            color: menuButton.color
            height: Consts.lineWidth
            width: 4*mm
        }

        Rectangle {
            id: rectangleBottom

            color: menuButton.color
            height: Consts.lineWidth
            width: 4*mm
        }
    }
}
