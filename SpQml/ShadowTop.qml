import QtQuick 2.7

import "./"
import "../"

Rectangle {
    id: shadowTop
    
    width: parent.width
    height: 1*mm
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#00000000"}
        GradientStop { position: 0.8; color: "#15000000"}
        GradientStop { position: 1.0; color: "#30000000"}
    }
}
