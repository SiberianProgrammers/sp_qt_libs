import QtQuick 2.7
import DXS 1.0
import "../"

//------------------------------------------------------------------------------
// Числовой счётчик. Анимирует увеличение и уменьшение числа.
//------------------------------------------------------------------------------
Item {
    id: _counter

    property int value

    width: main.width + _p.last.width
    height: first.height
    clip: true

    QtObject {
        id: _p

        property int lastValue: value
        property Text last: first
        property Text next: second
    }

    //--------------------------------------------------------------------------
    Text {
        id: main

        text: value
        font.pixelSize: Consts.fontNormal
    }

    //--------------------------------------------------------------------------
    Text {
        id: first

        font.pixelSize: Consts.fontNormal
        anchors.left: main.right
    }

    //--------------------------------------------------------------------------
    Text {
        id: second

        font.pixelSize: Consts.fontNormal
        y: parent.height
        anchors.left: main.right
    }

    //--------------------------------------------------------------------------
    SequentialAnimation {
        id: decreaseAnimation

        ParallelAnimation {
            NumberAnimation {
                target: _p.last
                property: "y"
                from: 0
                to: _counter.height
                duration: 100
            }
            NumberAnimation {
                target: _p.next
                property: "y"
                from: -_counter.height
                to: 0
                duration: 100
            }
        }
        PropertyAction {
            target: _p
            property: "last"
            value: _p.last === first ? second : first
        }
        PropertyAction {
            target: _p
            property: "next"
            value: _p.next === first ? second : first
        }
    }

    //--------------------------------------------------------------------------
    SequentialAnimation {
        id: increaseAnimation

        ParallelAnimation {
            NumberAnimation {
                target: _p.last
                property: "y"
                from: 0
                to: -_counter.height
                duration: 100
            }
            NumberAnimation {
                target: _p.next
                property: "y"
                from: _counter.height
                to: 0
                duration: 100
            }
        }
        PropertyAction {
            target: _p
            property: "last"
            value: _p.last === first ? second : first
        }
        PropertyAction {
            target: _p
            property: "next"
            value: _p.next === first ? second : first
        }
    }

    //--------------------------------------------------------------------------
    onValueChanged: {
        if (_p.lastValue === value) {
            return;
        }

        if (!increaseAnimation.running && !decreaseAnimation.running) {
            var lastValueText = _p.lastValue.toString();
            var valueText = value.toString();

            if (_p.lastValue > value) {
                for (var i=1; i<lastValueText.length && lastValueText[lastValueText.length-i] === "0"; ++i) {
                }
            } else {
                for (i=1; i<lastValueText.length && lastValueText[lastValueText.length-i] === "9"; ++i) {
                }
            }

            _p.last.text = lastValueText.substring(lastValueText.length-i);
            _p.next.text = valueText.substring(valueText.length-i);
            main.text = valueText.substring(0, valueText.length-i);


            if (_p.lastValue > value)  {
                decreaseAnimation.start();
            } else {
                increaseAnimation.start();
            }
        } else {
            _p.lastValue = value;
        }
    }
}
