import QtQuick 2.8
import "./"
import "../"

//------------------------------------------------------------------------------
/// @brief Объект, для создания всплывающих сообщений
/// Размещать в Root
/// TODO всю схему нужно переделать
/// TODO в данной реализации будет косяк с клавиатурой
//------------------------------------------------------------------------------
Loader {
    id: toast

    property string currentMessage: ""

    y: 0.7*parent.height - 0.5*height
    anchors.horizontalCenter: parent.horizontalCenter
    opacity: 0
    z: 10000

    Behavior on opacity { NumberAnimation { duration: 300 } }

    //--------------------------------------------------------------------------
    Component {
        id: visualComponent

        Rectangle {
            width : 0.7*Window.width
            height: messageText.contentHeight + 2 * Consts.margin
            color: "black"
            radius: Consts.radius

            Text {
                id: messageText
                text: currentMessage
                color: "white"
                font.pixelSize: Consts.fontSmall
                anchors.fill: parent
                anchors.margins: Consts.margin
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                maximumLineCount: 3
                wrapMode: Text.Wrap
            }
        }
    }

    //--------------------------------------------------------------------------
    Timer {
        id: closeTimer
        running: false
        interval: 1500
        repeat: false
        onTriggered: {
            toast.state = ""
        }
    }

    //--------------------------------------------------------------------------
    states: [
        State {
            name: "active"
            PropertyChanges {
                target: toast
                opacity: 0.7
            }
            PropertyChanges {
                target: toast
                sourceComponent: visualComponent
                restoreEntryValues: false
            }
        },
        State {
            name: ""
            PropertyChanges {
                target: toast
                opacity: 0
            }
        }
    ]

    //--------------------------------------------------------------------------
    // Функция показа всплывающего сообщения
    // showTime - время [мс] показа сообщения. По умолчанию 2500
    // forceShow - форсировать показ, даже если повторяет предыдущее
    //--------------------------------------------------------------------------
    function showMessage(text, showTime, force) {
        if (toast.state !== "") {
            return
        }

        currentMessage = text
        toast.state = "active";
        closeTimer.interval = 1500;
        if (showTime !== undefined) {
            closeTimer.interval = showTime
        }
        closeTimer.start();
    }
}
