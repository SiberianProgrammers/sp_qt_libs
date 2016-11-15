import QtQuick 2.7

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Протип кнопки. Обрабатывает сигналы нажатия, и т.п.
// @note Необходим для обхода узких мест, к примеру, в MouseArea нельзя послать
//       сигнал pressed() программно.
//------------------------------------------------------------------------------
Item {
    id: _buttonPrototype

    readonly property alias isPressed: mouseArea.pressed
    property real pressedX: 0
    property real pressedY: 0

    signal pressed
    signal released
    signal clicked
    signal pressedAndHold

    //--------------------------------------------------------------------------
    MouseArea {
        id: mouseArea

        anchors.fill: parent
        enabled: parent.enabled

        onPressed: {
            pressedX = mouse.x;
            pressedY = mouse.y;

            _buttonPrototype.pressed();
        }

        onReleased: {
            _buttonPrototype.released();
        }

        onClicked: {
            _buttonPrototype.clicked();
        }

        onPressAndHold: {
            _buttonPrototype.pressedAndHold();
        }
    }
}
