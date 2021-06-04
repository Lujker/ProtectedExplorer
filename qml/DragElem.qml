import QtQuick 2.0
import QtQml 2.15

Item {
    id: name
    property bool dragActive: false
    Drag.imageSource: "qrc:/../icons/move_file.png"
    Drag.dragType: Drag.Automatic
    Binding on Drag.active {
        value: dragActive
        delayed: true
    }
}
