import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: mainWindow
    minimumWidth: 820
    minimumHeight: 640
    visible: true
    title: qsTr("ProtectedExplorer")

    ExplorerrView {
        id: dirView
        listModel: DirModel
        width: parent.width / 2
        height: parent.height / 2
        onPressToElement: {

        }
        onOpenFolder: {
            DirModel.openFolder(index)
        }
    }

    ExplorerrView {
        id: subView
        listModel: SubModel
        width: parent.width / 2
        height: parent.height / 2
        anchors.left: dirView.right
        onPressToElement: {

        }
        onOpenFolder: {
            SubModel.openFolder(index)
        }
    }
}
