import QtQuick 2.0
import QtQml 2.15

Item {
    id: name
    property bool dragActive: false
    //    visible: false
    Drag.imageSource: "qrc:/../icons/move_file.png"
    Drag.dragType: Drag.Automatic
    //    Drag.active: dragActive
    Binding on Drag.active {
        value: dragActive
        delayed: true
    }

    //    Drag.onDragStarted: {
    //        visible = true
    //    }
    //    Drag.onDragFinished: {
    //        visible = false
    //    }
}
