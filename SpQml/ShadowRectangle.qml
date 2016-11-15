import QtQuick 2.5

import ".."

Item {
    id: _shadowRectangle

    property real step: size/countOfShards
    property real size: Consts.margin
    property real radius: Consts.radius
    property color color: Qt.rgba(0.5, 0.5, 0.5, 0.04)
    property int countOfShards: 8 // Количество прямоугольников для отрисовки тени

    Repeater {
        model: countOfShards
        
        Rectangle {
            id: _shadowShard
            
            readonly property real increment: index*step

            color: _shadowRectangle.color
            radius: _shadowRectangle.radius + index*index*step/10;
            anchors {
                fill: parent
                margins: -_shadowShard.increment
            }
        }
    }
}
