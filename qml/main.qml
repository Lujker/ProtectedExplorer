import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    minimumWidth: 820
    minimumHeight: 640
    visible: true
    property bool help: false
    title: qsTr("ProtectedExplorer")

    menuBar: MainMenuBar {
        id: mainAppMenu
    }

    ExplorerrView {
        id: dirView
        listModel: DirModel
        width: parent.width / 2
        height: parent.height
        onPressToElement: {
            subView.deselectAll()
        }
        onOpenFolder: {
            DirModel.openFolder(index)
        }
        onDeleteItem: {
            DirModel.deleteFiles(start, end)
        }
        onCopyElement: {
            DirModel.copySelections(start, end)
        }
        onPresToTable: {
            deselectAll()
            subView.deselectAll()
        }
    }

    ExplorerrView {
        id: subView
        listModel: SubModel
        width: parent.width / 2
        height: parent.height
        anchors.left: dirView.right
        onPressToElement: {
            dirView.deselectAll()
        }
        onOpenFolder: {
            SubModel.openFolder(index)
        }
        onDeleteItem: {
            SubModel.deleteFiles(start, end)
        }
        onCopyElement: {
            SubModel.copySelections(start, end)
        }

        onPresToTable: {
            deselectAll()
            dirView.deselectAll()
        }
    }
}
