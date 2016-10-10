import QtQuick 2.5

Item {
    property real margin: Math.ceil(2.4*mm)
    property real marginBig: Math.ceil(3.4*mm)
    property real spacing: Math.ceil(1*mm)
    property real marginCorner: Math.ceil(10*mm)
    property real fadeWidth: Math.ceil(5*mm)
    property real radius: Math.ceil(1*mm)
    property real lineWidth: Math.ceil(0.4*mm)
    property real borderWidth: Math.ceil(0.2*mm)

    property color gray: "#8a8a8a"
    property color darkGray: "#4a4a4a"
    property color lightGray: "#f0f0f0"
    property color green: "#71ad6f"
    property color lightGreen: "#d1ecd0"
    property color blue: "#0096d5"
    property color buttonActiveColor : "#72f7d6"
    property color buttonPassiveColor: "#c8e6df"
    property color borderColor: "#d9d9d9"
    property color shadowColor: "#dadada"
    property color backgroundColor: "#f0f0f0"

    //--------------------------------------------------------------------------
    // Шрифты
    //--------------------------------------------------------------------------
    //property alias mainFont: _mainFont
    property alias fontMetricsBig: _fontMetricsBig
    property alias fontMetricsNormal: _fontMetricsNormal
    property alias fontMetricsSmall: _fontMetricsSmall

    property real fontBig   : Math.ceil(5*mm)
    property real fontNormal: Math.ceil(3*mm)
    property real fontSmall : Math.ceil(2*mm)
    property real fontTiny  : Math.ceil(1*mm)
    property real fontBigHeight   : Math.ceil(1.73 * fontBig)
    property real fontNormalHeight: Math.ceil(1.73 * fontNormal)
    property real fontSmallHeight : Math.ceil(1.73 * fontSmall)
    property real fontTinyHeight  : Math.ceil(1.73 * fontTiny)
    property real fontNormalLetterSpacing: Math.ceil(0.2*mm)
    property real fontBigLetterSpacing   : Math.ceil(0.4*mm)
    property real fontSmallLetterSpacing : Math.ceil(0.1*mm)

    //--------------------------------------------------------------------------
    // ActionBar
    //--------------------------------------------------------------------------
    property real actionBarHeight: Math.ceil(
                                       Qt.platform.os === "ios"
                                       ? 7*mm
                                       : 9.5*mm )
    property color actionBarColor: "#7395FA"

    //--------------------------------------------------------------------------
    // Drawer
    //--------------------------------------------------------------------------
    property real drawerWidth: 40*mm
    property real drawerDragWidth: 2*mm

    //FontLoader { id: _mainFont; source: "qrc:/fonts/CTCSplashRounded.otf" }
    FontMetrics {
        id: _fontMetricsBig
        //font.family: _mainFont.name
        font.pixelSize: fontBig
    }
    FontMetrics {
        id: _fontMetricsNormal
        //font.family: _mainFont.name
        font.pixelSize: fontNormal
    }
    FontMetrics {
        id: _fontMetricsSmall
        //font.family: _mainFont.name
        font.pixelSize: fontSmall
    }
}
