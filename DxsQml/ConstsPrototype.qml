import QtQuick 2.5

Item {
    // Флаг, указывающий, что ОС - мобильная ОС.
    readonly property bool   mobileOs: Qt.platform.os === "ios" || Qt.platform.os === "android"
    readonly property alias  mm: _p.mm

    property real   margin: 3*mm
    property real   spacing: 2*mm
    property real   marginCorner: 10*mm
    property real   buttonRadius: 0.5*mm
    property real   buttonNormalWidth: _p.minWidth - 4*margin
    property real   fadeWidth: 5*mm

    property real   fontBig: 22
    property real   fontNormal: 17
    property real   fontSmall : 12
    property real   fontNormalLetterSpacing: 0.2*mm
    property real   fontBigLetterSpacing: 0.4*mm
    property real   fontSmallLetterSpacing: 0.1*mm

    property string gray: "#2a2a2a"
    property string lightGray: "#cacaca"
    property string buttonActiveColor : "#72f7d6"
    property string buttonPassiveColor: "#c8e6df"

    property real   borderWidth: 1*mm
    property string borderColor: "#d9d9d9"

    property alias mainFont: _mainFont
    property alias fontMetricsBig: _fontMetricsBig
    property alias fontMetricsNormal: _fontMetricsNormal
    property alias fontMetricsSmall: _fontMetricsSmall

    property real actionBarHeight: Qt.platform.os === "ios"
                                   ? 7*mm
                                   : 9.5*mm
    property string actionBarColor: "#7395FA"

    //--------------------------------------------------------------------------
    Item {
        id: _p

        property real mm
        property real minWidth
        property real maxHeight
    }

    FontLoader { id: _mainFont; source: "qrc:/fonts/CTCSplashRounded.otf" }
    FontMetrics {
        id: _fontMetricsBig
        font.family: _mainFont.name
        font.pointSize: fontBig
    }
    FontMetrics {
        id: _fontMetricsNormal
        font.family: _mainFont.name
        font.pointSize: fontNormal
    }
    FontMetrics {
        id: _fontMetricsSmall
        font.family: _mainFont.name
        font.pointSize: fontSmall
    }

    // Инициализирует константы согласно параметрам экрана и окна
    function init (screen, window) {
        _p.mm = screen.pixelDensity;
        _p.minWidth = Math.min (window.width, window.height);
        _p.maxHeight = Math.max (window.width, window.height);
    }
}
