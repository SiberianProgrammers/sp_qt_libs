import qbs
import qbs.FileInfo

Product {
    name: "sp_qt_libs"
    type: "staticlibrary"
    version: "1.0"

    files: [
        "include/Arc.h",
        "include/ArcFast.h",
        "include/Arrow.h",
        "include/DeviceInfo.h",
        "include/DownloadFileHandler.h",
        "include/ImageParallax.h",
        "include/ImageSaverSp.h",
        "include/ImageSp.h",
        "include/KeyboardSp.h",
        "include/LogSp.h",
        "include/Net.h",
        "include/Private/ImageSpLoader.h",
        "include/Private/ImageSpNode.h",
        "include/Private/NetHandler.h",
        "include/Settings.h",
        "include/Shadow.h",
        "include/SpApplicationPrototype.h",
        "source/Arc.cpp",
        "source/ArcFast.cpp",
        "source/Arrow.cpp",
        "source/DeviceInfo.cpp",
        "source/DownloadFileHandler.cpp",
        "source/ImageParallax.cpp",
        "source/ImageSaverSp.cpp",
        "source/ImageSp.cpp",
        "source/KeyboardSp.cpp",
        "source/LogSp.cpp",
        "source/Net.cpp",
        "source/Private/ImageSpLoader.cpp",
        "source/Private/ImageSpNode.cpp",
        "source/Private/NetHandler.cpp",
        "source/Settings.cpp",
        "source/Shadow.cpp",
        "source/SpApplicationPrototype.cpp",
        "qml/qml.qrc",
    ]

    cpp.includePaths: [path + "/include"]
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

      Export {
          Depends { name: "cpp" }
          cpp.includePaths: [path + "/include"]
     }
}
