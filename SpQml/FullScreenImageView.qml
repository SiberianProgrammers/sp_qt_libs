import QtQuick 2.7

import "./"
import "../"

ListView {
    id: _fullScreenImageView

    property KeysHandler keysHandler
    property var imagesList: []

    signal hide()
    signal show()

    width: Window.width
    height: Window.height
    model: imagesList.length
    interactive: visible
    enabled: visible
    visible: false
    snapMode: ListView.SnapOneItem
    highlightRangeMode: ListView.StrictlyEnforceRange
    orientation: ListView.Horizontal
    boundsBehavior: ListView.StopAtBounds
    cacheBuffer: 2*width

    delegate: FullScreenImage {
        width: Window.width
        height: Window.height
        photoSource: imagesList[index]
        visible: true
        downloadButtonVisible: false

        onClicked: {
            if (actionBarStateItem.state === "visible") {
                actionBarStateItem.state = "invisible"
            } else {
                actionBarStateItem.state = "visible"
            }

        }
    }

    Rectangle {
        width: parent.width
        height: Consts.statusBarHeight
        color: actionBar.color
    }

    //--------------------------------------------------------------------------
    ActionBar {
        id: actionBar

        title: _fullScreenImageView.imagesList.length > 1
                ?  qsTr("%1 из %2")
                    .arg(_fullScreenImageView.currentIndex+1)
                    .arg(_fullScreenImageView.imagesList.length)
                : ""

        titleColor: "white"
        color: Qt.rgba(0, 0, 0, 0.4);
        titleAlignment: Text.AlignHCenter
        width: Window.width
        shadowVisible: false
        y: Consts.statusBarHeight

        leftButton.sourceComponent: BackButton {
                id: backButton
                arrowColor: "white"
                y: Math.ceil((Consts.actionBarHeight - height)/2)
                z: actionBar.z

                Connections {
                    target: keysHandler
                    onBackKeyPressed: {
                        if (!context.accepted && backButton.enabled && backButton.visible) {
                            context.accepted = true;
                            _fullScreenImageView.backAction();
                        }
                    }
                }

                onClicked: {
                    _fullScreenImageView.backAction()
                }
        } // BackButton {

        rightButton.sourceComponent: MaterialButton {
            id: downloadButton

            color: "transparent"
            width: height-2*Consts.spacing
            radius: height/2
            border.width: 0
            pressedColor: "black"

            y: Math.ceil((Consts.actionBarHeight - height)/2)

            enabled: visible
            opacity: _fullScreenImageView.currentItem && _fullScreenImageView.currentItem.isReady
                     ? 1.0
                     : 0.4

            //--------------------------------------------------------------------------
            Image {
                source: "qrc:/download.png"

                readonly property int originalHeight: isDesktop
                                                      ? Math.ceil(2.7*mm)
                                                      : Math.ceil(2.5*mm)
                height: originalHeight % 2 === 0
                        ? originalHeight
                        : originalHeight + 1
                width: height
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            } // Image {

            onClicked: {
                _fullScreenImageView.saveImage()
            }
        } // MaterialButton {
    } // ActionBar {

    //--------------------------------------------------------------------------
    Item {
        id: actionBarStateItem

        state: "visible"
        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: actionBar
                    opacity: 1.0
                }
            }
            , State {
                name: "invisible"
                PropertyChanges {
                    target: actionBar
                    opacity: 0.0
                }
            }
        ]

        transitions: [
            Transition {
                from: "visible"
                to: "invisible"
                reversible: true

                NumberAnimation {
                    target: actionBar
                    property: "opacity"
                    easing.type: Easing.OutQuart
                    duration: 250
                }
            }
            ,Transition {
                to: "visible"
                from: "invisible"

                NumberAnimation {
                    target: actionBar
                    property: "opacity"
                    easing.type: Easing.OutQuart
                    duration: 150
                }
            }
        ]
    } // Item { id: actionBarStateItem

    //--------------------------------------------------------------------------
    function saveImage() {
        if (!imageSaverSp.checkFile(imagesList[_fullScreenImageView.currentIndex], Config.applicationCaption)) {
            var resultPath = imageSaverSp.save(imagesList[_fullScreenImageView.currentIndex], Config.applicationCaption);
            if (resultPath !== "") {
                toast.showMessage( qsTr("Фото сохранено в папке %1.").arg(Config.applicationCaption));
            } else {
                toast.showMessage( qsTr("Ошибка! Недостаточно памяти для сохранения фото."));
            }
        } else {
            toast.showMessage( qsTr("Фото уже сохранено в папке %1.").arg(Config.applicationCaption));
        }
    } // function saveImage() {

    //--------------------------------------------------------------------------
    function backAction() {
        hideFullscreen()
    } // function backAction() {

    //--------------------------------------------------------------------------
    function showFullscreen(imagesArr, currentIndex) {
        if (currentIndex === undefined || currentIndex < 0) {
            currentIndex = 0
        }

        _fullScreenImageView.imagesList = imagesArr
        _fullScreenImageView.positionViewAtIndex(currentIndex, ListView.Center)
        _fullScreenImageView.visible = true
        show()
    }

    //--------------------------------------------------------------------------
    function hideFullscreen() {
        _fullScreenImageView.visible = false
        hide()
    }
}
