import QtQuick 2.8

//
// Компонент горизонтального листания Swip'ом
//
// Инициализируется подобно ListView, за тем исключением, что в делегате
// нужно прописать свойство:
//   property QtObject model
// И обращаться к данным модели только через него.
//
// Перелистывание элементов к следующему, предыдущему производится функциями
// incrementCurrentIndex() и decrementCurrentIndex().
// Для пролистывания к конкретном элементу - функция
// setCurrentIndex (index, withAnimation)
//
//Item {
Rectangle {
    id: swipeAnimationDelegate

    transform: Translate { x: shift }

    property real shift
    property var loadedObject

    // Заглушка, препятсвующая нажатия внутри событий.
    MouseArea {
        anchors.fill: parent
        onClicked: {  }
    }

    // Затемнение соседних элементов
    Rectangle {
        id: fade
        color: "black"
        opacity: .0
        anchors.fill: parent
        z: 40
        scale: 1/parent.scale
    }

    // Тень справа
    Rectangle {
        id: shadow
        height: consts.ui_margin
        rotation: -90
        visible: false
        transformOrigin: Item.TopLeft
        width: swipeAnimationDelegate.height
        anchors {
            left: parent.right
            top: parent.bottom
        }

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#30000000"}
            GradientStop { position: 0.2; color: "#15000000"}
            GradientStop { position: 1.0; color: "#00000000"}
        }
    }

    states: [
        State {
            name: "active"
            
            PropertyChanges {
                target: shadow
                visible: true
                height: consts.ui_margin*(1 - listView.shiftXByWidth)
            }
        }
        ,State {
            name: "right"
            
            PropertyChanges {
                target: swipeAnimationDelegate
                z    : -1
                shift:  -width + listView.shiftX
                scale: 0.9 + 0.1*listView.shiftXByWidth
                opacity: 0.1 + 0.9*Math.pow(listView.shiftXByWidth,3)
            }
            PropertyChanges {
                target: fade
                opacity: 0.2*(1-listView.shiftXByWidth)
            }
        }
    ]

    property real diffIndex: index - listView.currentIndex;
    onDiffIndexChanged: {
        setState();
    }

    function setState () {
        switch (diffIndex) {
            case -1: state = ""; break;
            case +1: state = "right"; break;
            case  0: state = "active"; break;
            default: state = ""; break;
        }
    }

    Component.onCompleted: {
        setState();
    }
    
    Component.onDestruction: {
        //QmlHelper.destroyQMLObject (loadedObject);
    }
}
