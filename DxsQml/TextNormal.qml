import QtQuick 2.5
import "../"

Text {
    font {
        family: Consts.mainFont.name
        pointSize: Consts.fontNormal
        letterSpacing: Consts.fontNormalLetterSpacing
    }

    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    height: contentHeight
}
