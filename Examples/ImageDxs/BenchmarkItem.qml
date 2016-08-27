import QtQuick 2.5
import QtQuick.Window 2.2
import DXS 1.0

Item {
    id: _benchmarkItem

    property int size: 0

    anchors.fill: parent
    
    //--------------------------------------------------------------------------
    ImageDxs {
        id: image

        source: "qrc:/image.jpg"
        anchors.centerIn: parent
        fillMode: ImageDxs.PreserveAspectCrop
        width: benchmarkItem.width/2
        height: width
        radius: 20
    }
    
    //--------------------------------------------------------------------------
    MouseArea {
        anchors.fill: parent
        onClicked: {
            size = size < 2
                     ? size + 1
                     : 0;
        }
    }

    //--------------------------------------------------------------------------
    Item {
        id: visualStatusItem

        state: size
        states: [
            State {
                name: "0"

                PropertyChanges {
                    target: image
                    //width: benchmarkItem.width/2
                }
            } // State { name: "0"
            ,State {
                name: "1"

                PropertyChanges {
                    target: image
                    //width: _benchmarkItem.width - 2*margin
                    scale: 2.0
                }
            } // State { name: "1"
            ,State {
                name: "2"

                PropertyChanges {
                    target: image
                    //width: 3*_benchmarkItem.width
                    scale: 4.0
                }
            }// State { name: "2"
        ] // states: [ State {

        //----------------------------------------------------------------------
        transitions: [
            Transition {
                NumberAnimation {
                    properties: "width, height, scale"
                    duration: 300
                }
            }
        ] // transitions: [
    } // Item { id: visualStatusItem
}
