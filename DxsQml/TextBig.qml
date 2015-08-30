import QtQuick 2.5
import "../"

Text {
    font {
        family: Consts.mainFont.name
        pointSize: Consts.fontBig
        letterSpacing: Consts.fontBigLetterSpacing
    }
    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    height: contentHeight
}
