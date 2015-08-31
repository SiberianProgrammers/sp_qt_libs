import QtQuick 2.5

import "./"
import "../"

Rectangle {
    id: actionBar

    /// @brief Текст основной надписи.
    property alias mainCaption: mainCaption.text

    /// @brief Текст подстрочной надписи.
    property alias subCaption: subCaption.text

    /// @brief Компонент левой кнопки
    property alias leftButton: leftButton.sourceComponent

    /// @brief Компонент второй строки ActionBar'а
    property alias secondLine: secondLine.sourceComponent

    /// @brief Цвет надписи.
    property alias captionColor: mainCaption.color

    /// @brief Варавнивание надписи.
    property alias captionAlignment: mainCaption.horizontalAlignment

    /// @brief Флаг, указывающий на прозрачность.
    property bool transparent: false

    color: transparent ? "transparent" : Consts.actionBarColor
    height: firstLine.height + secondLine.height
    width: parent.width

    //--------------------------------------------------------------------------
    Item {
        id: firstLine

        width: parent.width
        height: Consts.actionBarHeight

        Loader {
            id: leftButton
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
        }

        Item {
            id: captionsContainer

            anchors {
                top: parent.top
                left: leftButton.right
                right: parent.right
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

                TextNormal {
                    id: mainCaption

                    color: "#555555"
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    font.weight: Font.Bold
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                TextSmall {
                    id: subCaption

                    color: mainCaption.color
                    horizontalAlignment: mainCaption.horizontalAlignment
                    elide: Text.ElideRight
                    wrapMode: Text.NoWrap
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: text === ""
                            ? 0
                            : contentHeight
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    Loader {
        id: secondLine

        anchors.top: firstLine.bottom
        width: parent.width
    }

    //--------------------------------------------------------------------------
    ShadowBottom {
        anchors.top: parent.bottom
        visible: !transparent
    }

    Component.onCompleted: {
    }
}
