import QtQuick 2.5
import "../"

Text {
    font {
        //family: Consts.mainFont.name
        pixelSize: Consts.fontNormal
        letterSpacing: Consts.fontNormalLetterSpacing
    }

    minimumPixelSize: 1
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
}
