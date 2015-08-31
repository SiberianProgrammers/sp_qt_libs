import QtQuick 2.5

import "./"
import "../"

Rectangle {
    id: shadowBottom
    
    width: parent.width
    height: 1*Consts.mm
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#30000000"}
        GradientStop { position: 0.2; color: "#15000000"}
        GradientStop { position: 1.0; color: "#00000000"}
    }
}
