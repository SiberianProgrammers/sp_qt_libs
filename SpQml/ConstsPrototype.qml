import QtQuick 2.8
import SP 1.0

Item {
    property double margin: Math.ceil(2.4*mm)
    property double marginBig: Math.ceil(3.4*mm)
    property double spacing: Math.ceil(1*mm)
    property double marginCorner: Math.ceil(10*mm)
    property double fadeWidth: Math.ceil(5*mm)
    property double radius: Math.ceil(1*mm)
    property double lineWidth: Math.ceil(0.4*mm)
    property double borderWidth: Math.ceil(0.2*mm)

    property color gray: "#c0c0c0"
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

    property double fontBig   : Math.ceil(5*mm)
    property double fontNormal: Math.ceil(3*mm)
    property double fontSmall : Math.ceil(2*mm)
    property double fontTiny  : Math.ceil(1*mm)
    property double fontBigHeight   : Math.ceil(1.73 * fontBig)
    property double fontNormalHeight: Math.ceil(1.73 * fontNormal)
    property double fontSmallHeight : Math.ceil(1.73 * fontSmall)
    property double fontTinyHeight  : Math.ceil(1.73 * fontTiny)
    property double fontNormalLetterSpacing: Math.ceil(0.2*mm)
    property double fontBigLetterSpacing   : Math.ceil(0.4*mm)
    property double fontSmallLetterSpacing : Math.ceil(0.1*mm)

    //--------------------------------------------------------------------------
    // ActionBar
    //--------------------------------------------------------------------------
    readonly property int _actionBarHeight: Qt.platform.os === "ios"
                                            ? 7*mm
                                            : 9.5*mm

    // Высота должна содержать нечётное количество пикселей.
    property int actionBarHeight: _actionBarHeight % 2 === 0
                                  ? _actionBarHeight
                                  : _actionBarHeight + 1
    property color actionBarColor: "#7395FA"

    property int statusBarHeight: deviceInfo.statusBarHeight()

    //--------------------------------------------------------------------------
    // Drawer
    //--------------------------------------------------------------------------
    property double drawerWidth: 40*mm
    property double drawerDragWidth: 2*mm

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
