import QtQuick 2.8

//------------------------------------------------------------------------------
// Компонент выбора свайпом, как в Tinder'е.
//
// Проще всего использовать - разместить SwipeSelector внутри объекта, отвечающего
// за отображения карточки выбора
//
// При смахивании влево вызывается сигнал leftSelected, вправо - rightSelected и т.д.
// factor [0..1], factorX, factorY [-1..+1] можно использовать для отображения
// выбора, к которому склоняется пользователь.
//------------------------------------------------------------------------------
MouseArea {
    id: _swipeSelector

    property Item target: parent
    property alias object: _swipeSelector.target

    // Начальные координаты карточки выбора
    property real originX: (target.parent.width-width)/2
    property real originY: (target.parent.height-height)/2

    // Порог относительного смещения по X и Y, после которого считается, что выбор произведён
    property real thresholdX: 0.3
    property real thresholdY: 0.3

    // Флаги, определяющие можно ли сделать выбор в соотвующем направлении
    property bool mayTopSelected: true
    property bool mayLeftSelected: true
    property bool mayRightSelected: true
    property bool mayBottomSelected: true

    // Обнуляет переменную target, если карточка выбранна.
    property bool nullTargerAfterRemoved: false

    // Относительное смещение в сторону того или иного варианта по X или Y [от -1 до 1].
    readonly property real factorX: diffX/maxX
    readonly property real factorY: diffY/maxY

    // Относительное смещение в любую сторону [от 0 до 1].
    readonly property real factor: Math.min(Math.max(Math.abs(swipeSelector.factorX), Math.abs(swipeSelector.factorY)), 1.0)

    // Сигнал о любом произведённом выборе - предшествует всем остальным сигналам
    signal selected

    // Сигналы о произведённом выборе свайпом
    signal topSelected
    signal leftSelected
    signal rightSelected
    signal bottomSelected

    // Сигнал о том, что карточка с выбором убрана после выбора
    signal cardRemoved

    readonly property real diffX: originX-target.x
    readonly property real diffY: originY-target.y
    readonly property real maxX: target.parent.width
    readonly property real maxY: target.parent.height

    anchors.fill: target
    drag {
        target: _swipeSelector.target
        axis: Drag.XAndYAxis
        minimumX: -_swipeSelector.width
        minimumY: -_swipeSelector.height
        maximumX: _swipeSelector.width
        maximumY: _swipeSelector.height
    }

    //--------------------------------------------------------------------------
    Rotation {
        id: targetRotation

        angle: origin.y > height/2
               ? factorX*40
               : -factorX*40
    }

    //--------------------------------------------------------------------------
    ParallelAnimation {
        id: resetAnimation

        NumberAnimation {
            property: "x"
            to: originX
            target: _swipeSelector.target
            easing.type: Easing.OutBack
            duration: 350
        }

        NumberAnimation {
            property: "y"
            to: originY
            target: _swipeSelector.target
            easing.type: Easing.OutBack
            duration: 350
        }
    }

    //----------------------------------------------------------------------
    Item {
        id: statesItem

        state: ""
        states: [
            State {
                name: "HiddenToLeft"
                PropertyChanges {
                    target: _swipeSelector.target
                    x: -1.3*Math.max(width, height)
                    y: y
                }
            }
            ,State {
                name: "HiddenToRight"
                PropertyChanges {
                    target: _swipeSelector.target
                    x: target.parent.width + 1.3*Math.max(width, height)
                    y: y
                }
            }
            ,State {
                name: "HiddenToTop"
                PropertyChanges {
                    target: _swipeSelector.target
                    x: x
                    y: -1.3*Math.max(width, height)
                }
            }
            ,State {
                name: "HiddenToBottom"
                PropertyChanges {
                    target: _swipeSelector.target
                    x: x
                    y: target.parent.height + 1.3*Math.max(width, height)
                }
            }
        ] // states: [

        //----------------------------------------------------------------------
        transitions: [
            Transition {
                to: ""
                PropertyAction {
                    properties: "x,y"
                }
            }
            // Анимация скрытия
            ,Transition {
                SequentialAnimation {
                    SmoothedAnimation {
                        properties: "x, y"
                        easing.type: Easing.OutBack
                        //duration: 550
                        velocity: 6*Math.min (width, height)
                    }
                    ScriptAction {
                        script: {
                            if (nullTargerAfterRemoved) {
                                target.transform = null;
                                target = null;
                            }

                            statesItem.state = "";
                            cardRemoved();
                        }
                    }
                }
            }
        ] // transitions: [
    } // Item { id: statesItem

    //--------------------------------------------------------------------------
    onTargetChanged: {
        if (target !== null) {
            target.transform = targetRotation;
        }
    }

    //--------------------------------------------------------------------------
    onPressed: {
        targetRotation.origin.x = mouseX;
        targetRotation.origin.y = mouseY;
    }

    onReleased: {
        if (mayBottomSelected && factorY < -thresholdY) {
            _swipeSelector.hideToBottom();
            selected();
            bottomSelected();
        } else if (mayTopSelected && factorY > thresholdY) {
            _swipeSelector.hideToTop();
            selected();
            topSelected();
        } else if (mayLeftSelected && factorX > thresholdX) {
            _swipeSelector.hideToLeft();
            selected();
            leftSelected();
        } else if (mayRightSelected && factorX < -thresholdX) {
            _swipeSelector.hideToRight();
            selected();
            rightSelected();
        } else {
            _swipeSelector.resetPosition();
        }
    }

    //----------------------------------------------------------------------
    function resetPosition() {
        resetAnimation.start();
    }

    function hideToLeft() {
        statesItem.state = "HiddenToLeft";
    }

    function hideToRight() {
        statesItem.state = "HiddenToRight";
    }

    function hideToTop() {
        statesItem.state = "HiddenToTop";
    }

    function hideToBottom() {
        statesItem.state = "HiddenToBottom";
    }
} // MouseArea { id: dragArea
