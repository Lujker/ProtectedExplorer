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
    menuBar: mainAppMenu
    DropArea {
        id: _dropArea
        anchors.fill: parent
        MainMenuBar {
            id: mainAppMenu
        }

        AppButton {
            id: _signature
            anchors.top: _dropArea.top
            anchors.horizontalCenter: _dropArea.horizontalCenter
            anchors.topMargin: 14
            text: "Подписать"
            toolTipText: "Подписать выбранные файлы"
        }
        AppButton {
            id: _send
            anchors.top: _signature.bottom
            anchors.topMargin: 14
            anchors.left: dirView.right
            text: "Отправить"
            toolTipText: "Отправить выбранные файлы (F5)"
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
                SubModel.addFile()
            }
            onAddNewFolder: {
                SubModel.addFolder()
            }
        }
    }
}
