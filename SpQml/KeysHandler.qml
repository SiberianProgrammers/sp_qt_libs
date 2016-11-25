import QtQuick 2.8

//------------------------------------------------------------------------------
// Обработчик нажатий на кнопки (в том числе кнопку "Назад" в Android).
//------------------------------------------------------------------------------
Item {
    id: _keysHandler

    signal backKeyPressed (var context)
    signal keyPressed (var context,var event)

    focus: true

    Keys.onReleased: {
        event.accepted = true
        var context = {
            accepted: false
            , key: event.key
        };

        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
            backKeyPressed(context);

            if (!context.accepted) {
                if (_p.isExit) {
                    Qt.quit()
                } else {
                    _p.isExit = true
                    exit_timer.start()
                    toast.showMessage(qsTr("Для выхода из приложения нажмите \"Назад\" еще раз."))
                }
            }
        } else {
            keyPressed (context, event);
        }

        event.accepted = true
    }// Keys.onReleased: {

    //------------------------------------------------------------------------------
    Timer {
        id: exit_timer
        interval: 1500
        running: !_p.isExit
        onTriggered: {
            _p.isExit = false;
        }
    } // Timer {

    //------------------------------------------------------------------------------
    QtObject {
        id: _p

        property bool isExit: false
    }
}
