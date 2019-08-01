import QtQuick 2.8

//------------------------------------------------------------------------------
// Предоставляет механизм увеличения для заданного компонента
//------------------------------------------------------------------------------
Flickable {
    id: zoom

    property alias source: contentLoader.source
    property alias sourceComponent: contentLoader.sourceComponent
    property alias item: contentLoader.item
    property real  sidesRatio: item != null
                               ? item.sidesRatio
                               : 1
    property real  maximumScale: 3.0
    readonly property bool zoomed: _p.zoomed
    property bool  pinchActive: false
    property bool  isFullScreen: true

    // При запрете зума возвращаем в исходное состояние!
    property bool zoomDenied: false

    signal clicked(var mouse)

    boundsBehavior: Flickable.StopAtBounds
    contentWidth: width
    contentHeight: height

    onZoomDeniedChanged: {
        if (zoomDenied) {
            pinchArea.enabled = false
            mouseArea.state = "normal"
        }
    }
    //--------------------------------------------------------------------------
    PinchArea {
        id: pinchArea

        property real initialWidth
        property real initialHeight

        width : Math.max(zoom.contentWidth, zoom.width)
        height: Math.max(zoom.contentHeight, zoom.height)
        enabled: isFullScreen && !timerForEnabled.running

        //----------------------------------------------------------------------
        onPinchStarted: {
            pinchActive = true;
            // чтобы можно было даже из состояния normal перейти в normal
            mouseArea.state = "nonnormal"
            initialWidth = zoom.contentWidth
            initialHeight = zoom.contentHeight
            zoom.interactive = false;
        } // onPinchStarted: {

        //----------------------------------------------------------------------
        onPinchUpdated: {
            zoom.contentX += pinch.previousCenter.x - pinch.center.x;
            zoom.contentY += pinch.previousCenter.y - pinch.center.y;

            var scaledWidth = pinch.scale * initialWidth;
            if (scaledWidth < zoom.width) {
                zoom.resizeContent(zoom.width, zoom.height, pinch.center);
            } else if (scaledWidth < maximumScale*zoom.width) {
                zoom.resizeContent(scaledWidth, Math.max(scaledWidth/sidesRatio, zoom.height), pinch.center);
            } else {
                zoom.resizeContent(maximumScale*zoom.width, maximumScale*zoom.width/sidesRatio, pinch.center);
            }
        }// onPinchUpdated: {

        //----------------------------------------------------------------------
        onPinchFinished: {
            pinchActive = false;
            zoom.returnToBounds()
            zoom.interactive = true;

            // Доводка мелкого пинча
            var widthRatio = Math.abs(zoom.width - zoom.contentWidth);
            if (widthRatio < 5*mm) {
                zoom.contentWidth = width;
                _p.zoomed = false;
            } else {
                _p.zoomed = true;
            }
        }// onPinchFinished: {

        //----------------------------------------------------------------------
        Loader {
            id: contentLoader

            width: parent.width
            height: parent.height
            asynchronous: true
        } // Loader { id: contentLoader

        //----------------------------------------------------------------------
        MouseArea {
            id: mouseArea

            property int mouseX: 0
            property int mouseY: 0

            enabled: isFullScreen && !timerForEnabled.running
            anchors.fill: parent

            onDoubleClicked: {
                if (pinchActive || zoomDenied) {
                    return;
                }

                doubleClickTimer.stop();
                doubleClickTimer.clickMouse = null;
                if (state === "normal") {
                    mouseX = mouse.x
                    mouseY = mouse.y
                }

                state = zoomed
                        ? "normal"
                        : "zoomed"
                _p.zoomed = !_p.zoomed;
            }

            state: "normal"
            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: zoom
                        contentWidth: zoom.width
                        contentHeight: zoom.height
                        contentX: 0
                        contentY: 0
                    }
                }
                ,State {
                    name: "zoomed"
                    PropertyChanges {
                        target: zoom
                        contentWidth: maximumScale*zoom.width
                        contentHeight: maximumScale*zoom.width/sidesRatio

                        contentX: zoom.contentWidth / zoom.width* mouseArea.mouseX - zoom.width / 2 < 0
                                    ? 0
                                    : Math.min(zoom.contentWidth / zoom.width*mouseArea.mouseX - zoom.width/2, zoom.contentWidth - zoom.width )
                        contentY: zoom.contentHeight / zoom.height * mouseArea.mouseY - zoom.height / 2 < 0
                                    ? 0
                                    : Math.min(zoom.contentHeight / zoom.height*mouseArea.mouseY - zoom.height/2, zoom.contentHeight - zoom.height )
                    }
                }
            ] // states: [

            transitions: [
                Transition {
                    SequentialAnimation {
                        NumberAnimation {
                            properties: "contentWidth, contentHeight, contentX, contentY"
                            duration: 200
                        }
                        ScriptAction {
                            script: {
                                zoom.returnToBounds();
                            }
                        }
                    }
                }
            ] // transitions: [

            onClicked: {
                doubleClickTimer.clickMouse = mouse;
                doubleClickTimer.start();
            }

            Timer {
                id: doubleClickTimer
                property var clickMouse: undefined;

                running     : false
                interval    : 200
                onTriggered : {
                    zoom.clicked(clickMouse);
                    clickMouse = null;
                }
            } // Timer { id: doubleClickTimer
        } // MouseArea { id: mouseArea
    } // PinchArea { id: pinchArea

    //--------------------------------------------------------------------------
    Timer {
        id: timerForEnabled
        running: isFullScreen
        interval: 300
    }

    //--------------------------------------------------------------------------
    Item {
        id: _p

        property bool zoomed: false
    }
}
