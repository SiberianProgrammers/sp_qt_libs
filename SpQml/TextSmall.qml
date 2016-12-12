import QtQuick 2.7
import "../"

Text {
    font {
        //family: Consts.mainFont.name
        pixelSize: Consts.fontSmall
        letterSpacing: Consts.fontSmallLetterSpacing
    }

    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    height: contentHeight
}
