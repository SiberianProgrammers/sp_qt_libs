import qbs
import qbs.FileInfo

Product {
    name: "sp_qt_libs"
    type: "staticlibrary"
    version: "1.0"

    files: [
        "Include/Arc.h",
        "Include/ArcFast.h",
        "Include/Arrow.h",
        "Include/DeviceInfo.h",
        "Include/DownloadFileHandler.h",
        "Include/ImageParallax.h",
        "Include/ImageSaverSp.h",
        "Include/ImageSp.h",
        "Include/KeyboardSp.h",
        "Include/LogSp.h",
        "Include/Net.h",
        "Include/Private/ImageSpLoader.h",
        "Include/Private/ImageSpNode.h",
        "Include/Private/NetHandler.h",
        "Include/Settings.h",
        "Include/Shadow.h",
        "Include/SpApplicationPrototype.h",
        "Source/Arc.cpp",
        "Source/ArcFast.cpp",
        "Source/Arrow.cpp",
        "Source/DeviceInfo.cpp",
        "Source/DownloadFileHandler.cpp",
        "Source/ImageParallax.cpp",
        "Source/ImageSaverSp.cpp",
        "Source/ImageSp.cpp",
        "Source/KeyboardSp.cpp",
        "Source/LogSp.cpp",
        "Source/Net.cpp",
        "Source/Private/ImageSpLoader.cpp",
        "Source/Private/ImageSpNode.cpp",
        "Source/Private/NetHandler.cpp",
        "Source/Settings.cpp",
        "Source/Shadow.cpp",
        "Source/SpApplicationPrototype.cpp",
        "SpQml/SpQml.qrc",
    ]

    cpp.includePaths: ["Include"]
    cpp.cxxLanguageVersion: "c++11"
    qbs.architecture: "x86_64"

    Depends { name: "Qt"; submodules: ["core", "network", "qml", "quick" ] }
    Depends { name: "cpp" }

    Group {
        name: "install"
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installPrefix: "lib"
        qbs.installSourceBase: qbs.buildDirectory
    }

    Properties {
        condition: qbs.targetOS.contains("macos")
        // Грязный хак для доступа к приватным классам QtPlatformHeaders. Используется в ObjCFunctions.mm
        cpp.includePaths: [
            Qt.core.libPath,
            Qt.core.libPath + "/QtGui.framework/Versions/" + Qt.core.versionMajor + "/Headers/" + Qt.core.version
        ]
        cpp.minimumMacosVersion: "10.7"
    }
}
