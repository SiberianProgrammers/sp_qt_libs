import QtQuick 2.7

import "./"
import "../"

Rectangle {
    id: actionBar

    /// @brief Текст основной надписи.
    property alias title: title.text

    /// @brief Текст подстрочной надписи.
    property alias subTitle: subTitle.text

    /// @brief Цвет надписи.
    property alias titleColor: title.color

    /// @brief Варавнивание надписи.
    property alias titleAlignment: title.horizontalAlignment

    /// @brief Компонент левой кнопки
    property alias leftButton: leftButton

    /// @brief Компонент правой кнопки
    property alias rightButton: rightButton

    /// @brief Компонент второй строки ActionBar'а
    property alias secondLine: secondLine.sourceComponent

    /// @brief Флаг, указывающий на прозрачность.
    property bool transparent: false

    property alias shadowVisible: shadow.visible

    color: transparent ? "transparent" : Consts.actionBarColor
    height: firstLine.height + secondLine.height
    width: parent.width

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.top
        }
        height: Consts.statusBarHeight
        color: parent.color
    }

    //--------------------------------------------------------------------------
    Item {
        id: firstLine

        width: parent.width
        height: Consts.actionBarHeight
        anchors.top: parent.top

        //----------------------------------------------------------------------
        // Левая кнопка
        //----------------------------------------------------------------------
        Loader {
            id: leftButton
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
        }

        //----------------------------------------------------------------------
        // Заголовок
        //----------------------------------------------------------------------
        Item {
            id: titlesContainer

            anchors {
                top: parent.top
                left: leftButton.right
                right: rightButton.left
                bottom: parent.bottom
            }

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Consts.spacing
                    leftMargin: leftButton.width ? 0 : Consts.spacing
                    verticalCenter: parent.verticalCenter
                }

                Text {
                    id: title

                    color: "#555555"
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    minimumPixelSize: 1
                    fontSizeMode: Text.HorizontalFit
                    height: contentHeight
                    font {
                        pixelSize: Consts.fontTitle
                        letterSpacing: Consts.fontTitleLetterSpacing
                        weight: Font.Bold
                    }
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                Text {
                    id: subTitle

                    color: title.color
                    horizontalAlignment: title.horizontalAlignment
                    elide: Text.ElideRight
                    font {
                        pixelSize: Consts.fontSmall
                        letterSpacing: Consts.fontSmallLetterSpacing
                    }
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: text === ""
                            ? 0
                            : contentHeight
                }
            }
        } // Item { id: titlesContainer

        //----------------------------------------------------------------------
        // Правая кнопка
        //----------------------------------------------------------------------
        Loader {
            id: rightButton
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
        }
    } // Item { id: firstLine

    //--------------------------------------------------------------------------
    Loader {
        id: secondLine

        anchors.top: firstLine.bottom
        width: parent.width
    }

    //--------------------------------------------------------------------------
    ShadowBottom {
        id: shadow

        anchors.top: parent.bottom
        visible: !transparent
    }
}
