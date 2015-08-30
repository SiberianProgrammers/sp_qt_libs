import QtQuick 2.5

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Прототип кнопки с материальным дизайном (анимация фона при нажатии)
//------------------------------------------------------------------------------
ButtonPrototype {
    id: materialButton

    property alias activeColor: background.color
    property alias pressedColor: pressedPlace.color

    Rectangle {
        id: background

        color: Consts.buttonActiveColor
        anchors.fill: parent
    }

    //--------------------------------------------------------------------------
    // TODO При нажатии сделать, чтобы углы оставались закгруглёнными
    Rectangle {
        id: pressedPlace

        x: pressedX
        y: pressedY
        width: 3*radius
        height: width
        color: "#000000"
        opacity: 0.1
        transform: Translate {
            x: -pressedPlace.width/2
            y: -pressedPlace.height/2
        }
    }

    //--------------------------------------------------------------------------
    Item {
        id: statesItem

        states: [
            State {
                when: isPressed
                name: "pressed"
                PropertyChanges {
                    target: pressedPlace
                    radius: materialButton.width
                }
            }
        ]

        transitions: [
            Transition {
                to: "pressed"
                NumberAnimation {
                    target: pressedPlace
                    property: "radius"
                    duration: 400
                }
            }
            ,Transition {
                from: "pressed"
                NumberAnimation {
                    target: pressedPlace
                    property: "radius"
                    duration: 200
                }
            }
        ]
    }
}
