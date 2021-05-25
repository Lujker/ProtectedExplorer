﻿import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

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
        AppComboBox {
            id: dirComboBox
            currentIndex: 0
            width: dirView.width / 2
            anchors.top: _dropArea.top
            anchors.left: dirView.left
            onActivated: {
                switch (index) {
                case 0:
                    DirModel.setAsDirModel()
                    break
                case 1:
                    DirModel.setAsSubModel()
                    break
                default:
                    return
                }
            }
        }

        AppComboBox {
            id: subComboBox
            width: subView.width / 2
            anchors.top: _dropArea.top
            currentIndex: 1
            anchors.left: subView.left
            onActivated: {
                switch (index) {
                case 0:
                    SubModel.setAsSubModel()
                    break
                case 1:
                    SubModel.setAsDirModel()
                    break
                default:
                    return
                }
            }
        }

        ExplorerrView {
            id: dirView
            parent: _dropArea
            listModel: DirModel
            width: parent.width / 2
            height: parent.height - _rowLayoutTableButtom.height - dirComboBox.height
            anchors.top: dirComboBox.bottom
            anchors.bottom: _rowLayoutTableButtom.top
            onPressToElement: {
                subView.deselectAll()
            }
            onOpenFolder: {
                listModel.openFolder(index)
            }
            onDeleteItem: {
                listModel.deleteFile(index)
            }
            onCopyElement: {
                listModel.copyFile(index)
            }
            onPresToTable: {
                deselectAll()
                subView.deselectAll()
            }
            onAddNewFile: {
                if (focusOfView === true)
                    listModel.addFile()
            }
            onAddNewFolder: {
                if (focusOfView === true)
                    listModel.addFolder()
            }
        }

        ExplorerrView {
            id: subView
            parent: _dropArea
            listModel: SubModel
            width: parent.width / 2
            height: parent.height - _rowLayoutTableButtom.height - subComboBox.height
            anchors.top: subComboBox.bottom
            anchors.bottom: _rowLayoutTableButtom.top
            anchors.right: parent.right
            onPressToElement: {
                dirView.deselectAll()
            }
            onOpenFolder: {
                listModel.openFolder(index)
            }
            onDeleteItem: {
                listModel.deleteFile(index)
            }
            onCopyElement: {
                listModel.copyFile(index)
            }
            onPresToTable: {
                deselectAll()
                dirView.deselectAll()
            }
            onAddNewFile: {
                if (focusOfView === true)
                    listModel.addFile()
            }
            onAddNewFolder: {
                if (focusOfView === true)
                    listModel.addFolder()
            }
        }

        RowLayout {
            id: _rowLayoutTableButtom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            AppButton {
                icon: "qrc:/../icons/add_file.png"
                toolTipText: "Создать файл в открытом каталоге таблицы"
                onButtonClicked: {
                    subView.addNewFile()
                    dirView.addNewFile()
                }
            }
            AppButton {
                icon: "qrc:/../icons/add-folder.png"
                toolTipText: "Создать папку в открытом каталоге таблицы"
                onButtonClicked: {
                    subView.addNewFolder()
                    dirView.addNewFolder()
                }
            }
            AppButton {
                icon: "qrc:/../icons/delete_file.png"
                toolTipText: "Удалить выбранные в таблице файлы (delete/backspace)"
                onButtonClicked: {
                    subView.deleteSelected()
                    dirView.deleteSelected()
                }
            }
            AppButton {
                icon: "qrc:/../icons/sent.png"
                toolTipText: "Копировать выбранный файлы в другую таблицу (F5)"
                onButtonClicked: {
                    subView.sendFiles()
                    dirView.sendFiles()
                }
            }
            AppButton {
                id: _signature
                icon: "qrc:/../icons/edit_file.png"
                toolTipText: "Подписать выбранные файлы"
            }
        }
    }
}
