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
    property alias radius: background.radius

    Rectangle {
        id: background

        color: Consts.buttonActiveColor
        anchors.fill: parent
        clip: true

        //--------------------------------------------------------------------------
        // TODO При нажатии сделать, чтобы углы оставались закгруглёнными
        //--------------------------------------------------------------------------
        Rectangle {
            id: pressedPlace

            readonly property real rightShift: pressedX+width/2 - background.width
            readonly property real leftShift: width/2 - pressedX
            readonly property bool inEdge: rightShift > 0 || leftShift > 0

            x: rightShift > 0
               ? pressedX - rightShift
               : leftShift > 0
                 ? pressedX + leftShift
                 : pressedX
            y: parent.height/2
            height: Math.min(width, background.height)
            color: "#000000"
            opacity: 0.1
            radius: inEdge
                    ? background.radius
                    : height/2
            transform: Translate {
                x: -pressedPlace.width/2
                y: -pressedPlace.height/2
            }
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
                    //radius: materialButton.width/2
                    width: background.width
                }
            }
        ]

        transitions: [
            Transition {
                to: "pressed"
                NumberAnimation {
                    target: pressedPlace
                    property: "width"
                    duration: 400
                }
            }
            ,Transition {
                from: "pressed"
                NumberAnimation {
                    target: pressedPlace
                    property: "width"
                    duration: 200
                }
            }
        ]
    }
}
