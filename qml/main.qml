import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: mainWindow
    minimumWidth: 1020
    minimumHeight: 640
    visible: true
    property bool help: true
    title: qsTr("Модуль взаимодействия со шлюзом обмена данными")
    menuBar: mainAppMenu

    ///Статичный член для перетаскивания
    DragElem {
        parent: _dropArea
        id: draggable
    }

    Dialog {
        id: _dialogNewFile
        title: "Введите имя для файла"
        height: 100
        width: 200

        TextField {
            id: _txtFieldFile
            anchors.fill: parent
            anchors.centerIn: parent.Center
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            dirView.addNewFile(_txtFieldFile.text)
            subView.addNewFile(_txtFieldFile.text)
        }
    }
    Dialog {
        id: _dialogNewFolder
        title: "Введите имя для папки"
        height: 100
        width: 200

        TextField {
            id: _txtFieldFolder
            anchors.fill: parent
            anchors.centerIn: parent.Center
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            dirView.addNewFolder(_txtFieldFolder.text)
            subView.addNewFolder(_txtFieldFolder.text)
        }
    }

    DropArea {
        id: _dropArea
        anchors.fill: parent
        MainMenuBar {
            id: mainAppMenu
        }
        ///Комбо боксы для имзиенения отображений таблиц
        AppComboBox {
            id: dirComboBox
            currentIndex: 0
            width: 180
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
            width: 180
            anchors.top: _dropArea.top
            currentIndex: 1
            anchors.left: subView.left
            onActivated: {
                switch (index) {
                case 0:
                    SubModel.setAsDirModel()
                    break
                case 1:
                    SubModel.setAsSubModel()
                    break
                default:
                    return
                }
            }
        }

        ///Отображение таблицы (1) (по-умолчанию доступных пользовательских папок)
        ExplorerrView {
            id: dirView
            parent: _dropArea
            listModel: DirModel
            height: parent.height - _rowLayoutTableButtom.height - dirComboBox.height
            anchors.top: dirComboBox.bottom
            anchors.bottom: _rowLayoutTableButtom.top
            anchors.right: _rectWidth.left
            anchors.left: parent.left
            anchors.leftMargin: 5
            onPressToElement: {
                subView.deselectAll()
            }
            onOpenFolder: {
                listModel.openFolder(index)
            }
            onDeleteItem: {
                listModel.deleteFile(index)
            }
            onMoveItem: {
                listModel.moveFile(index)
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
                    listModel.addFile(name)
            }
            onAddNewFolder: {
                if (focusOfView === true)
                    listModel.addFolder(name)
            }
            onSignedElement: {
                if (focusOfView === true)
                    listModel.signedFile(index)
            }
            onStrartDragElem: {
                draggable.dragActive = true
                subView.deselectAll()
            }
            onEndDragElem: {
                draggable.dragActive = false
                if (dirView.curDir.length !== 0) {
                    subView.moveElement()
                }
            }
        }
        ///Элемент для изменения ширины таблиц
        DragWidthRect {
            id: _rectWidth
            x: mainWindow.width / 2
            height: parent.height - _rowLayoutTableButtom.height - subComboBox.height - 20
            anchors.top: subComboBox.bottom
        }

        ///Отображение таблицы (2) (по-умолчанию сетевых папок)
        ExplorerrView {
            id: subView
            parent: _dropArea
            listModel: SubModel
            height: parent.height - _rowLayoutTableButtom.height - subComboBox.height
            anchors.top: subComboBox.bottom
            anchors.bottom: _rowLayoutTableButtom.top
            anchors.left: _rectWidth.right
            anchors.rightMargin: 5
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
            onMoveItem: {
                listModel.moveFile(index)
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
                    listModel.addFile(name)
            }
            onAddNewFolder: {
                if (focusOfView === true)
                    listModel.addFolder(name)
            }
            onSignedElement: {
                if (focusOfView === true)
                    listModel.signedFile(index)
            }
            onStrartDragElem: {
                draggable.dragActive = true
                dirView.deselectAll()
            }
            onEndDragElem: {
                draggable.dragActive = false
                if (subView.curDir.length !== 0) {
                    dirView.moveElement()
                }
            }
        }

        ///строка с кнопками
        RowLayout {
            id: _rowLayoutTableButtom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            AppButton {
                icon: "qrc:/../icons/add_file.png"
                toolTipText: "Создать файл в открытом каталоге таблицы"
                onButtonClicked: {
                    _dialogNewFile.open()
                }
            }
            AppButton {
                icon: "qrc:/../icons/add-folder.png"
                toolTipText: "Создать папку в открытом каталоге таблицы"
                onButtonClicked: {
                    _dialogNewFolder.open()
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
                onButtonClicked: {
                    subView.signedFiles()
                    dirView.signedFiles()
                }
            }
        }
    }
}
