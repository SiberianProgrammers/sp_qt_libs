import QtQuick 2.8

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
    property color textDisabledColor: "#88000000"
    property alias color: background.color
    property alias pressedColor: pressedPlace.color
    property color disabledColor: color
    property alias radius: background.radius
    property alias border: background.border
    property alias pressedPlaceOpacity: pressedPlace.opacity

    property int durationAnimationPressed: 400
    property int durationAnimationReleased: 200

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

            readonly property double rightShift: pressedX+width/2 - background.width
            readonly property double leftShift: width/2 - pressedX
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
            border {
                width: background.border.width
                color: background.border.color
            }
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
                name: "disabled"
                when: !_materialButton.enabled
                PropertyChanges {
                    target: textItem
                    color: textDisabledColor
                }
                PropertyChanges {
                    target: _materialButton
                    color: disabledColor
                }
            }
            ,State {
                name: "pressed"
                when: isPressed
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
                        duration: durationAnimationPressed
                    }
                }
            }
            ,Transition {
                from: "pressed"
                NumberAnimation {
                    target: pressedPlace
                    property: "width"
                    duration: durationAnimationReleased
                }
            }
        ]
    }
}
