import QtQuick 2.5
import SP 1.0

Item {
    id: _contactDelegate

    property bool expanded: false

    width: listView.width
    height: 16*mm

    //--------------------------------------------------------------------------
    ImageSp {
        id: avatar

        property real r: width/2
        source: "qrc:/avatar.jpg"
        radius: r
        x: margin
        y: (parent.height-avatar.height)/2
        height: parent.height - margin
        width: height

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!expanded) {
                    viaElement.x = _contactDelegate.mapToItem(root,0,0).x;
                    viaElement.y = _contactDelegate.mapToItem(root,0,0).y;
                }
                expanded = !expanded;
            }
        }
    }

    //--------------------------------------------------------------------------
    Text {
        id: nameItem

        text: index + " Маргарита Ивановна Сушина"
        anchors {
            left: parent.left
            right: parent.right
            margins: margin
            leftMargin: parent.height + margin
            verticalCenter: parent.verticalCenter
        }
        elide: Text.ElideRight
    }

    //--------------------------------------------------------------------------
    Item {
        id: statesItem

        state: expanded
               ? "expanded"
               : "normal"
        states: [
            State {
                name: "normal"

                PropertyChanges {
                    target: avatar
                    r: avatar.height/2
                }
                ParentChange {
                    target: avatar
                    parent: _contactDelegate
                }
                PropertyChanges {
                    target: containerBackground
                    explicit: true
                    opacity: 0.0
                }
            }
            ,State {
                name: "expanded"

                PropertyChanges {
                    target: avatar
                    r: 0
                }
                ParentChange {
                    target: avatar
                    parent: container
                    x: margin
                    y: (parent.height-avatar.height)/2
                    width: parent.width - 2*margin
                    height: width
                }
                PropertyChanges {
                    target: containerBackground
                    explicit: true
                    opacity: 1.0
                }
            }
        ] // states: [

        transitions: [
            Transition {
                SequentialAnimation {
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "r,opacity"
                            duration: 300
                        }
                        ParentAnimation {
                            via: viaElement
                            NumberAnimation {
                                properties: "x,y,width,height"
                                duration: 300
                            }
                        }
                    } // ParallelAnimation {
                }
            }
        ]
    } // Item { id: statesItem
}
