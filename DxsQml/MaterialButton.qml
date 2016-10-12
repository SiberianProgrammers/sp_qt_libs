import QtQuick 2.7

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Прототип кнопки с материальным дизайном (анимация фона при нажатии)
//------------------------------------------------------------------------------
ButtonPrototype {
    id: _materialButton

    property alias text: textItem.text
    property alias font: textItem.font
    property alias textItem: textItem
    property alias color: background.color
    property alias pressedColor: pressedPlace.color
    property alias radius: background.radius

    width: textItem.width + 2*Consts.margin

    Rectangle {
        id: background

        color: Consts.buttonActiveColor
        anchors.fill: parent
        clip: true
        radius: Consts.radius

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
            color: "gray"
            opacity: 0.1
            radius: inEdge
                    ? background.radius
                    : height/2
            transform: Translate {
                x: -pressedPlace.width/2
                y: -pressedPlace.height/2
            }
        }
    } // Rectangle { id: background

    //--------------------------------------------------------------------------
    Text {
        id: textItem

        anchors.centerIn: parent
        font.pixelSize: Consts.fontNormal
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
                SequentialAnimation {
                    NumberAnimation {
                        target: pressedPlace
                        property: "width"
                        easing.type: Easing.InQuad
                        duration: 400
                    }
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
