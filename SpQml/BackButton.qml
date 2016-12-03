import QtQuick 2.8
import SP 1.0

import "./"
import "../"

//------------------------------------------------------------------------------
// @brief Кнопка назад
//------------------------------------------------------------------------------
MaterialButton {
    id: backButton

    property string arrowColor: "white"
    property real yShift: 0

    color: "transparent"
    border.width: 0
    width: height-2*Consts.spacing
    radius: height/2
    pressedColor: "black"

    Arrow {
        id: arrow

        readonly property int originalHeight: isDesktop
                                              ? Math.ceil(2.8*mm)
                                              : Math.ceil(2.5*mm)
        height: originalHeight % 2 === 0
                ? originalHeight
                : originalHeight + 1
        width: height
        color: "white"
        penWidth: Consts.lineWidth
        antialiasing: true
        x: Math.ceil((parent.width-width)/2)
        y: Math.round((parent.height-height)/2) + yShift
    }
}
