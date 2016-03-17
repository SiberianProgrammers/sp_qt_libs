import QtQuick 2.5
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
