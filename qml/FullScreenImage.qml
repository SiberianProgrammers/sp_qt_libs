import QtQuick 2.8
import SP 1.0

import "./"
import "../"

Item {
    id: _fullScreenPhoto

    property string photoSource: ""
    property alias downloadButtonVisible: downloadButton.visible
    readonly property bool isReady: scaleblePhoto.item !== null && scaleblePhoto.item.isReady

    signal clicked()

    //--------------------------------------------------------------------------
    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    //--------------------------------------------------------------------------
    Zoom {
        id: scaleblePhoto

        sourceComponent: Item {
            property alias sidesRatio: _onlineImage.sidesRatio
            property alias isReady   : _onlineImage.isReady

            ImageSp {
                id: _onlineImage

                readonly property bool isReady: status === ImageSp.Ready
                readonly property real sidesRatio: sourceSize.height !== 0
                                          ? sourceSize.width / sourceSize.height
                                          : 1

                fillMode: Image.PreserveAspectFit
                source: photoSource
                scale: isReady
                       ? parent.width/width < parent.height/height
                         ? parent.width/width
                         : parent.height/height
                       : 1
                anchors.centerIn: parent
            }
        }

        onClicked: {
            _fullScreenPhoto.clicked()
        }

        width: _fullScreenPhoto.width
        height: _fullScreenPhoto.height
    }//Zoom { id: scaleblePhoto

    //--------------------------------------------------------------------------
    // Кнопка сохранить
    //--------------------------------------------------------------------------
    Image {
        id: downloadButton

        anchors {
            top  : parent.top
            right: parent.right
            rightMargin: Consts.margin
            topMargin: 1.2*Consts.margin + Consts.statusBarHeight
        }
        height: 2*Consts.margin
        width: height
        enabled: visible

        // Так делаь плохо. Подумать, что сделать с изображениями в sp_qt_libs
        source: "qrc:/download.png"
        opacity: scaleblePhoto.item !== null && scaleblePhoto.item.isReady
                 ? 1.0
                 : 0.4

        MouseArea {
            anchors.fill: parent
            anchors.margins: -Consts.margin
            enabled: parent.visible
            onClicked: {
                downloadButton.saveImage()
            }
        }

        function saveImage() {
            if (!imageSaverSp.checkFile(_fullScreenPhoto.photoSource)) {
                var resultPath = imageSaverSp.save(_fullScreenPhoto.photoSource);

                if (resultPath !== "") {
                    toast.showMessage( qsTr("Фото сохранено"));
                } else {
                    toast.showMessage( qsTr("Ошибка! Недостаточно памяти для сохранения фото."));
                }
            }
        }
    } // Image { id: downloadButton
}




















































