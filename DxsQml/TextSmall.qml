import QtQuick 2.5
import "../"

Text {
    font {
        //family: Consts.mainFont.name
        pointSize: Consts.fontSmall
        letterSpacing: Consts.fontSmallLetterSpacing
    }

    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    height: contentHeight
}
