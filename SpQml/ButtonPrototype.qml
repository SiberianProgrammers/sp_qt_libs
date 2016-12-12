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
    property double pressedX: 0
    property double pressedY: 0
    property double margins: -Consts.spacing

    signal pressed
    signal released
    signal clicked
    signal pressedAndHold

    //--------------------------------------------------------------------------
    MouseArea {
        id: mouseArea

        anchors {
            fill: parent
            margins: _buttonPrototype.margins
        }
        enabled: parent.enabled

        onPressed: {
            pressedX = Math.min(Math.max(mouse.x + margins, 0), _buttonPrototype.width);
            pressedY = Math.min(Math.max(mouse.y + margins, 0), _buttonPrototype.height);

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
