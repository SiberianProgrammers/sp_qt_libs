import QtQuick 2.5

import "qrc:/DxsQml"
import "./"
import "../"

Rectangle {
    id: drawer

    readonly property alias shown: _p.shown;

    width: Consts.drawerWidth
    height: parent.height

    Behavior on x {
        SmoothedAnimation { velocity: width/0.3}
    }

    //--------------------------------------------------------------------------
    MouseArea {
        id: maForShowAndHide

        anchors {
            top: parent.top
            left: parent.right
            bottom: parent.bottom
        }

        drag.target: drawer
        drag.axis: Drag.XAxis
        drag.minimumX: -drawer.width
        drag.maximumX: 0

        onReleased: {
            if (shown) {
                if (drawer.x < -drawer.width/3) {
                    _p.shown = false;
                } else {
                    drawer.x = 0
                }
            } else {
                if (drawer.x > -drawer.width*2/3) {
                    _p.shown = true;
                } else {
                    drawer.x = -drawer.width
                }
            }
        }

        onClicked: {
            if (shown) {
                hide();
            }
        }
    }

    //--------------------------------------------------------------------------
    Rectangle {
        id: fade

        opacity: _p.shift
        color: "#4d000000"
        anchors {
            top: parent.top
            left: parent.right
            bottom: parent.bottom
        }
        width: Consts.maxWidth
    }

    //--------------------------------------------------------------------------
    Rectangle {
        id: shadow

        visible: drawer.x !== -drawer.width
        width: parent.height
        height: 1*Consts.mm
        rotation: -90
        anchors {
            left: parent.right
            top: parent.bottom
        }
        transformOrigin: Item.TopLeft
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#30000000"}
            GradientStop { position: 0.2; color: "#15000000"}
            GradientStop { position: 1.0; color: "#00000000"}
        }
    }

    //--------------------------------------------------------------------------
    Item {
        id: _p

        property bool shown: false;
        property real shift: 1 + drawer.x/drawer.width
    }

    //--------------------------------------------------------------------------
    Item {
        id: statesItem

        state: "hidden"
        states: [
            State {
                name: "hidden"
                when: !drawer.shown

                PropertyChanges {
                    target: drawer
                    x: -width
                }
                PropertyChanges {
                    target: maForShowAndHide
                    width: Consts.drawerDragWidth
                }
            },
            State {
                name: "shown"
                when: drawer.shown

                PropertyChanges {
                    target: drawer
                    x: 0
                }
                PropertyChanges {
                    target: shadow
                    visible: true
                }
                PropertyChanges {
                    target: maForShowAndHide
                    width: Consts.maxWidth
                }
            }
        ]
    }

    //--------------------------------------------------------------------------
    function show() {
        _p.shown = true;
    }

    //--------------------------------------------------------------------------
    function hide() {
        _p.shown = false;
    }
}
