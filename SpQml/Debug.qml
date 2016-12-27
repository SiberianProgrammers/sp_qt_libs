pragma Singleton

import QtQuick 2.7

Item {
    function printObject (data, header){
        var printTop = false
        if (header === undefined) {
            printTop = false
            header = "**********************"
        } else {
            header = " " + header + " "
        }

        var filler = ""
        for (var i = header.length + 6; i > 0; --i) {
            filler += "*"
        }

        if (printTop) {
            console.log("/" + filler + "/")
        }

        console.log("/***" + header + "***/")
        _p.printObjectInternal(data)
        console.log("/" + filler + "/")
    }

    //--------------------------------------------------------------------------
    Item {
        id: _p

        function printObjectInternal(data, prefix, level)
        {
            if (typeof prefix === 'undefined' || typeof prefix === 'null') {
                prefix = "  ";
            }
            if (typeof level === 'undefined' || typeof prefix === 'null') {
                level = 0;
            } else if (level > 5)
                return;

            var indentStr = ""
            for (var i = 0; i < level; ++i) {
                indentStr += prefix;
            }

            if (Object.prototype.toString.call(data) == '[object Object]') {
                var funcs = []
                for (var key in data) {
                    if (Object.prototype.toString.call(data[key]) == '[object Object]') {
                        if (key == 'parent' || key == 'objectName' || data[key] === data) {
                            continue;
                        }

                        console.log(indentStr + key + "     " + data[key])
                        printObjectInternal(data[key], prefix , level + 1);
                    } else if (Object.prototype.toString.call(data[key]) == '[object Function]') {
                        if (key.slice(0,2) !== "__") {
                            funcs.push(key)
                        }
                        continue;
                    } else if (Object.prototype.toString.call(data[key]) == '[object Array]') {
                        console.log(indentStr + key + " [Array]");
                        printObjectInternal(data[key], prefix, level + 1)
                    } else {
                        console.log (indentStr + key + " = " + data[key] + " [" + typeof(data[key]) + "]");
                    }
                }

                if (funcs.length !== 0) {
                    console.log(indentStr + "Functions:")
                    for (key in funcs) {
                        console.log(indentStr + prefix + funcs[key])
                    }
                }
            } else if (Object.prototype.toString.call(data) == '[object Array]') {
                for (i = 0; i < data.length; ++i) {
                    if (data[i] == undefined) {
                        continue;
                    }

                    console.log(indentStr + "[" + i + "]")
                    printObjectInternal(data[i], prefix, level + 1)
                }
            } else {
                var str = "";
                str += indentStr + Object.prototype.toString.call(data) + "   " + data;
                console.log (indentStr + data + " [" + typeof(data) + "]");
            }
        }
    }
}

