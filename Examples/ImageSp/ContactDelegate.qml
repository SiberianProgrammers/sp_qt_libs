import QtQuick 2.5
import SP 1.0

Item {
    id: _contactDelegate

    width: listView.width
    height: Math.ceil(avatar.height + margin)

    //--------------------------------------------------------------------------
    ImageSp {
        id: avatar

        source: "qrc:/avatar.jpg"
        width: Math.ceil(10*mm)
        height: width
        radius: width/2
        asynchronous: true
        anchors {
            left: parent.left
            leftMargin: margin
            verticalCenter: parent.verticalCenter
        }
    }

    //--------------------------------------------------------------------------
    Text {
        id: nameItem

        text: index + " Маргарита Ивановна Сушина"
        anchors {
            left: avatar.right
            right: parent.right
            margins: margin
            verticalCenter: parent.verticalCenter
        }
        elide: Text.ElideRight
    }
}
