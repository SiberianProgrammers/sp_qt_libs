import QtQuick 2.8
import QtQuick.Window 2.2

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
Item {
    id: swipeView

    property alias model   : listView.model
    property alias delegate: listView.delegate

    property double initialIndex: 0 // Начальный индекс с которого начинается просмотр
    readonly property alias activeIndex: listView._activeIndex // Активный индекс, т.е. на котором остановился SwipeView
    readonly property alias currentIndex: listView._currentIndex // Текущий индекс элемента. @note Может несколько раз поменяться после setCurrentIndex
    readonly property alias currentItem: listView.currentItem // Текущий элемент в списке
    readonly property alias isMoving: listView.isMoving // Флаг того, что листание в процессе.
    property double maximumFlickVelocity: 3 * width // Скорость анимации пролистывания
    property alias animationVelocity: swipeView.maximumFlickVelocity
    property double maxAnimationDuration: 400 // Максимальная длительность анимации пролистывания
    property bool interactive: true // Флаг интерактивность swipeView
    property bool withAnimation: false
    readonly property alias count: listView.count
    readonly property alias movedBySwipe: listView.movedBySwipe // Флаг того, что currentIndex поменялся в результате изменения swip'ом
    readonly property alias shiftX: listView.shiftX
    readonly property alias shiftXByWidth: listView.shiftXByWidth
    //TODO Вынести в параметры ориентацию списка (сейчас только горизонтальная)

    //--------------------------------------------------------------------------
    ListView {
        id: listView

        property bool isMoving: false
        property bool movedBySwipe: false
        property int _currentIndex: Math.floor((diffX + width/2)/ width)
        property int _activeIndex: 0
        readonly property double inEdge: diffX % width === 0
        readonly property double diffX: contentX - originX
        readonly property double shiftX: diffX - (_currentIndex)*width
        readonly property double shiftXByWidth: shiftX / width

        signal needSetState

        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        maximumFlickVelocity: swipeView.maximumFlickVelocity
        anchors.fill: parent
        visible: initialIndex !== 0
        boundsBehavior: Flickable.StopAtBounds
        interactive: swipeView.interactive //&& !flicking
        cacheBuffer: 2*width

        onDragStarted: {
            movedBySwipe = true;
        }

        onInEdgeChanged: {
            if (inEdge) {
                interactive = true && swipeView.interactive;
            }
            isMoving = !inEdge;
        }

        onIsMovingChanged: {
            if (!isMoving) {
                _activeIndex = _currentIndex;
            }
        }

        Behavior on contentX {
            id: behOnContentX
            enabled: false

            SequentialAnimation {
                SmoothedAnimation {
                    velocity: listView.highlightMoveVelocity
                    duration: maxAnimationDuration
                }
                ScriptAction {
                    script: {
                        //print ("Animation stoped, contentX: "+listView.contentX);
                        behOnContentX.enabled = false;
                    }
                }
            }
        }

        //--------------------------------------------------------------------------
        Component.onCompleted: {
            if (initialIndex > 0 && initialIndex < model.count) {
                //contentX = initialIndex*width+originX;
                positionViewAtIndex(initialIndex, ListView.Beginning);
                //_currentIndex = initialIndex;
            }
            visible = true;
        }
    }

    //--------------------------------------------------------------------------
    // Делегат с включённой анимацией
    Component {
        id: animationDelegateComponent
        SwipeAnimationDelegate { }
    }

    //--------------------------------------------------------------------------
    onModelChanged: {
        //listView._currentIndex = 0;
    }

    onInteractiveChanged: {
        listView.interactive = interactive;
    }

    //--------------------------------------------------------------------------
    // Функции для работы со SwipeView
    //--------------------------------------------------------------------------
    //
    // Переход к следующему элементу
    function incrementCurrentIndex () {
        if (!interactive || isMoving) {
            return;
        }

        if (listView._currentIndex+1 < model.count) {
            //listView.isMoving = true;
            listView.movedBySwipe = false;
            behOnContentX.enabled = true;
            listView.contentX += width;
        } else {
            Log.warning ("It's already at end of SwipeView");
        }
    }

    //--------------------------------------------------------------------------
    // Переход к предыдущему элементу
    function decrementCurrentIndex () {
        if (!interactive || isMoving) {
            return;
        }

        if (listView._currentIndex >= 1) {
            //listView.isMoving = true;
            listView.movedBySwipe = false;
            behOnContentX.enabled = true;
            listView.contentX -= width;
        } else {
            Log.warning ("It's already at begining of SwipeView");
        }
    }

    //--------------------------------------------------------------------------
    // Установка _currentIndex в конкретное значение index.
    // Если установлен флаг withAnimation в true, то переход происходит
    // с анимацией, иначе - без.
    function setCurrentIndex (index, withAnimation/* = false*/) {
        if (!interactive || isMoving) {
            return;
        }

        var contentX = index*width+listView.originX;

        if (listView.contentX !== contentX) {
            listView.movedBySwipe = false;

            if (withAnimation !== undefined && withAnimation) {
                listView.isMoving = true;
                behOnContentX.enabled = true;
            }

            listView.contentX = contentX;
        } else {
            Log.warning ("It's already at this index");
        }
    }
}
