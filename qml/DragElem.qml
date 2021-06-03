import QtQuick 2.0

Item {
    id: name
    property bool dragActive: false
    visible: false
    Drag.imageSource: "qrc:/../icons/add_file.png"
    Drag.dragType: Drag.Automatic
    Drag.active: dragActive
    Drag.onDragStarted: {
        visible = true
    }
    Drag.onDragFinished: {
        visible = false
    }
}
