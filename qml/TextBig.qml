import QtQuick 2.8
import "../"

Text {
    font {
        //family: Consts.mainFont.name
        pixelSize: Consts.fontBig
        letterSpacing: Consts.fontBigLetterSpacing
    }
    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    height: contentHeight
}
