import QtQuick 2.5
import "../"

Text {
    font {
        family: Consts.mainFont.name
        pointSize: Consts.fontNormal
        letterSpacing: Consts.fontNormalLetterSpacing
    }

    minimumPointSize: 1
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
}
