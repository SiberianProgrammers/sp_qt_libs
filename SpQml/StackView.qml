import QtQuick 2.7
import "../"
import "./"

// Контейнер, подгружающий компоненты один за другим.
Item {
    id: stackView

    // Массив компонентов для отображения.
    property var components

    // Индекс начального компонента для загрузки
    property int initialComponent: 0

    // Количество компонентов в массиве.
    readonly property int count: components.length

    // Текущий индекс отображаемого компонента.
    readonly property alias currentIndex: _p.currentIndex

    // Индекс элемента, который станет следующим. Выставляется перед сменой компонентов.
    readonly property alias nextCurrentIndex: _p.nextCurrentIndex

    // Сигнал о началае смены компонентов.
    // \param currentIndex - индекс текущего элемента, до смены компонентов
    // \param nextCurrentIndex - индекс следующиего текущего элемента, после смены
    signal startSwitch (var currentIndex, var nextCurrentIndex)

    // Сигнал об окончании смены компонентов.
    // \param currentIndex - индекс текущего элемента, после сменыт компонентов
    // \param previousCurrentIndex - индекс предыдущего текущего элемента
    signal switched (var currentIndex, var previousCurrentIndex)

    //--------------------------------------------------------------------------
    Loader {
        id: firstLoader

        property int index: 0

        sourceComponent: stackView.components[initialComponent]
        anchors.fill: parent
    }

    Loader {
        id: secondLoader

        property int index

        anchors.fill: parent
    }

    //--------------------------------------------------------------------------
    Item {
        id: statesItem

        states: [
            State {
                name: "firstLoader"
                PropertyChanges {
                    target: firstLoader
                    visible: true
                }
                PropertyChanges {
                    target: secondLoader
                    visible: false
                }
            }
            ,State {
                name: "secondLoader"
                PropertyChanges {
                    target: firstLoader
                    visible: false
                }
                PropertyChanges {
                    target: secondLoader
                    visible: true
                }
            }
        ]
    }

    //--------------------------------------------------------------------------
    Item {
        id: _p

        property int currentIndex: 0
        property int nextCurrentIndex: 0
    }

    //--------------------------------------------------------------------------
    // Меняет текущий отображемый компонент с следующий в списке
    function next() {
        if (_p.currentIndex+1 >= count) {
            return
        }

        var previousCurrentIndex = _p.currentIndex;

        ++_p.nextCurrentIndex;

        startSwitch (_p.currentIndex, _p.nextCurrentIndex)

        if (statesItem.state === "firstLoader") {
            secondLoader.index = nextCurrentIndex;
            secondLoader.sourceComponent = components[_p.nextCurrentIndex];
            statesItem.state = "secondLoader";
            firstLoader.sourceComponent = null;
        } else {
            firstLoader.index = nextCurrentIndex;
            firstLoader.sourceComponent = components[_p.nextCurrentIndex];
            statesItem.state = "fistLoader";
            secondLoader.sourceComponent = null;
        }

        _p.currentIndex = _p.nextCurrentIndex;
        switched (_p.currentIndex, previousCurrentIndex);
    }
}
