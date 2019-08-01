import QtQuick 2.5
import "qrc:/SpQml"

Rectangle {
    id: debugDelegate

    property bool onTop: true
    readonly property alias swipeSelector: swipeSelector

    x: swipeSelector.originX
    y: swipeSelector.originY
    radius: 2*mm
    width: parent.width-2*margin
    height: parent.height-20*mm
    color: "#4ba4db"
    z: onTop
       ? 0
       : -1
    scale: onTop
           ? 1.0
           : 0.8 + 0.2*swipeSelectorTop.factor

    //----------------------------------------------------------------------
    SwipeSelector {
        id: swipeSelector

        mayBottomSelected: false

        onTopSelected: print ("top selected")
        onLeftSelected: print ("left selected")
        onRightSelected: print ("right selected")
        onBottomSelected: print ("bottom selected")

        onCardRemoved: {
            blueOnTop = !blueOnTop;
            x = swipeSelector.originX;
            y = swipeSelector.originY;
        }
    } // Item { id: swipeSelector

    //--------------------------------------------------------------------------
    Text {
        text: "right"
        anchors {
            top: parent.top
            left: parent.left
            margins: margin
        }
        font {
            bold: true
            pointSize: 17
        }
        opacity: onTop
                 ? -swipeSelector.factorX
                 : 0.0
    }
    
    Text {
        text: "left"
        anchors {
            top: parent.top
            right: parent.right
            margins: margin
        }
        font {
            bold: true
            pointSize: 17
        }
        opacity: onTop
                 ? swipeSelector.factorX
                 : 0.0
    }
    
    Text {
        text: "top"
        anchors {
            bottom: parent.bottom
            margins: margin
            horizontalCenter: parent.horizontalCenter
        }
        font {
            bold: true
            pointSize: 17
        }
        opacity: onTop
                 ? swipeSelector.factorY
                 : 0.0
    }
    
    Text {
        text: "bottom"
        anchors {
            top: parent.top
            margins: margin
            horizontalCenter: parent.horizontalCenter
        }
        font {
            bold: true
            pointSize: 17
        }
        opacity: onTop
                 ? -swipeSelector.factorY
                 : 0
    }
}
