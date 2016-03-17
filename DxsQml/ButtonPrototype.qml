import QtQuick 2.5

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Протип кнопки. Обрабатывает сигналы нажатия, и т.п.
//------------------------------------------------------------------------------
Item {
    id: buttonPrototype

    property bool isPressed: false
    property real pressedX: 0
    property real pressedY: 0

    signal pressed
    signal released
    signal clicked

    //--------------------------------------------------------------------------
    MouseArea {
        anchors.fill: parent

        onPressed: {
            pressedX = mouse.x;
            pressedY = mouse.y;

            isPressed = true;
            buttonPrototype.pressed();
        }

        onReleased: {
            isPressed = false;
            buttonPrototype.released();
        }

        onClicked: {
            buttonPrototype.clicked();
        }
    }
}
