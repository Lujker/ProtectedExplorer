import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    minimumWidth: 1020
    minimumHeight: 640
    visible: true
    property bool help: false
    title: qsTr("ProtectedExplorer")

    menuBar: MainMenuBar {
        id: mainAppMenu
    }

    AppButton {
        id: _signature
        anchors.top: mainAppMenu.bottom
        anchors.horizontalCenter: mainAppMenu.horizontalCenter
        anchors.topMargin: 14
        text: "Подписать"
    }
    AppButton {
        id: _send
        anchors.top: _signature.bottom
        anchors.topMargin: 14
        anchors.left: dirView.right
        text: "Отправить"
        onButtonClicked: {
            dirView.sendFiles()
        }
    }

    ExplorerrView {
        id: dirView
        listModel: DirModel
        width: parent.width / 2 - _signature.width / 2
        height: parent.height
        anchors.right: _signature.left
        onPressToElement: {
            subView.deselectAll()
        }
        onOpenFolder: {
            DirModel.openFolder(index)
        }
        onDeleteItem: {
            DirModel.deleteFile(index)
        }
        onCopyElement: {
            DirModel.copyFile(index)
        }
        onPresToTable: {
            deselectAll()
            subView.deselectAll()
        }
        onAddNewFile: {
            DirModel.addFile()
        }
        onAddNewFolder: {
            DirModel.addFolder()
        }
    }

    ExplorerrView {
        id: subView
        listModel: SubModel
        width: parent.width / 2 - _signature.width / 2
        height: parent.height
        anchors.left: _signature.right
        onPressToElement: {
            dirView.deselectAll()
        }
        onOpenFolder: {
            SubModel.openFolder(index)
        }
        onDeleteItem: {
            SubModel.deleteFile(index)
        }
        onCopyElement: {
            SubModel.copyFile(index)
        }
        onPresToTable: {
            deselectAll()
            dirView.deselectAll()
        }
        onAddNewFile: {
            DirModel.addFile()
        }
        onAddNewFolder: {
            SubModel.addFolder()
        }
    }
}
