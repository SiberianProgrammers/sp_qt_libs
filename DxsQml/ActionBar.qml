import QtQuick 2.5

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
    property alias leftButton: leftButton.sourceComponent

    /// @brief Компонент второй строки ActionBar'а
    property alias secondLine: secondLine.sourceComponent

    /// @brief Флаг, указывающий на прозрачность.
    property bool transparent: false

    property alias shadowVisible: shadow.visible

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
            id: titlesContainer

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
                    id: title

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
                    id: subTitle

                    color: title.color
                    horizontalAlignment: title.horizontalAlignment
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
        id: shadow

        anchors.top: parent.bottom
        visible: !transparent
    }

    Component.onCompleted: {
    }
}
