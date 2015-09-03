import QtQuick 2.5

import "./"
import "../"

Rectangle {
    id: actionBarImage

    property alias shift: _p.shift

    /// @brief Источник изображения
    property alias source: image.source

    /// @brief Высота изображения
    property real imageHeight: 30*Consts.mm

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

    /// @brief Координата y прокручиваемого содержимого
    property real contentY: 0
    
    color: Consts.actionBarColor
    width: parent.width
    height: Math.max (Consts.actionBarHeight, imageHeight-contentY)

    //--------------------------------------------------------------------------
    Image {
        id: image
        
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        opacity: 1-_p.shift
    }

    //--------------------------------------------------------------------------
    Loader {
        id: leftButton
        anchors {
            top: parent.top
            left: parent.left
        }
        height: Consts.actionBarHeight
    }

    //--------------------------------------------------------------------------
    Item {
        id: titlesContainer

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: Consts.margin + (leftButton.width-Consts.margin)*_p.shift
        }
        height: Consts.actionBarHeight

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

    //--------------------------------------------------------------------------
    ShadowTop {
        anchors.bottom: parent.bottom
        opacity: 1-_p.shift
    }

    ShadowBottom {
        anchors.top: parent.bottom
        opacity: _p.shift
    }

    //--------------------------------------------------------------------------
    Item {
        id: _p

        readonly property real shift: 1 -(actionBarImage.height - Consts.actionBarHeight)/(imageHeight - Consts.actionBarHeight)
    }
}
