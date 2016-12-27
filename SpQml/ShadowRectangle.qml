import QtQuick 2.7

import ".."

Item {
    id: _shadowRectangle

    property double step: size/countOfShards
    property double size: Consts.margin
    property double radius: Consts.radius
    property color color: Qt.rgba(0.5, 0.5, 0.5, 0.04)
    property int countOfShards: 8 // Количество прямоугольников для отрисовки тени

    Repeater {
        model: countOfShards
        
        Rectangle {
            id: _shadowShard
            
            readonly property double increment: index*step

            color: _shadowRectangle.color
            radius: _shadowRectangle.radius + index*index*step/10;
            anchors {
                fill: parent
                margins: -_shadowShard.increment
            }
        }
    }
}
