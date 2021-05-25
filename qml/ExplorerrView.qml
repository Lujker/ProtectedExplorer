import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.0

Rectangle {
    id: explWindow
    property alias listModel: fileList.model
    property bool focusOfView: fileList.focus
    property bool inputName: false
    signal pressToElement(int index)
    signal copyElements(int start, int end)
    signal copyElement(int index)
    signal deleteItems(int start, int end)
    signal deleteItem(int index)
    signal signedElements(int start, int end)
    signal signedElement(int index)
    signal openFolder(int index)
    signal renameFile(int index, string newName)
    signal presToTable
    signal addNewFile
    signal addNewFolder

    TableView {
        id: fileList
        parent: explWindow
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: dataModel
        clip: true
        property bool shiftPressed: false
        property bool controlPressed: false

        TableViewColumn {
            role: "filepath"
            title: "Иконка"
            delegate: Component {
                id: imageDelegate
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    cache: true
                    asynchronous: true
                    sourceSize.width: 8
                    sourceSize.height: 8
                    source: styleData.value ? "image://iconProvider/" + styleData.value : ""
                }
            }
            width: 50
        }
        TableViewColumn {
            role: "name"
            title: "Имя"
            width: explWindow.width - 341
        }
        TableViewColumn {
            role: "suffix"
            title: "Тип"
            width: 50
        }
        TableViewColumn {
            role: "size"
            title: "Размер"
            width: 70
        }
        TableViewColumn {
            role: "date"
            title: "Дата"
            width: 150
        }
        sortIndicatorVisible: true

        onSortIndicatorColumnChanged: {
            listModel.setSorting(sortIndicatorColumn, 0)
        }
        onSortIndicatorOrderChanged: {
            listModel.setSorting(sortIndicatorColumn, sortIndicatorOrder)
        }
        DropArea {
            anchors.fill: parent
            onEntered: {

            }
        }
        rowDelegate: Rectangle {
            id: _rowDelegate
            color: styleData.selected ? "skyblue" : "white"

            MouseArea {
                id: _dragArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    fileList.focus = true
                    if (mouse.button === Qt.LeftButton) {
                        if (fileList.controlPressed) {
                            fileList.selection.select(styleData.row)
                        } else if (fileList.shiftPressed) {
                            var lowIndex = fileList.rowCount + 1
                            fileList.selection.forEach(function (rowIndex) {
                                if (lowIndex > rowIndex)
                                    lowIndex = rowIndex
                            })
                            fileList.selection.select(lowIndex, styleData.row)
                        } else {
                            deselectAll()
                            pressToElement(styleData.row)
                            fileList.selection.select(styleData.row)
                        }
                    } else if (mouse.button === Qt.RightButton) {
                        deselectAll()
                        pressToElement(styleData.row)
                        fileList.selection.select(styleData.row)
                        _itemDelegatePopup.open()
                    }
                }
                onPressAndHold: {
                    console.debug("Press and hold")
                    fileList.selection.select(styleData.row)
                }
                onDoubleClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        deselectAll()
                        openFolder(styleData.row)
                    }
                }
            }
        }

        itemDelegate: Component {

            Item {
                id: fileDelegate
                clip: true
                TextInput {
                    id: _txtInput
                    anchors.left: parent.left
                    readOnly: styleData.column !== 1
                    text: styleData.value
                    renderType: Text.NativeRendering
                    onAccepted: {
                        inputName = false
                        listModel.renameFile(styleData.row, text)
                        parent.forceActiveFocus()
                        console.debug(text)
                        console.debug(styleData.row)
                    }
                }

                MouseArea {
                    id: _dragArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        inputName = false
                        parent.forceActiveFocus()
                        fileList.focus = true
                        if (mouse.button === Qt.LeftButton) {
                            if (fileList.controlPressed) {
                                fileList.selection.select(styleData.row)
                            } else if (fileList.shiftPressed) {
                                var lowIndex = fileList.rowCount + 1
                                fileList.selection.forEach(function (rowIndex) {
                                    if (lowIndex > rowIndex)
                                        lowIndex = rowIndex
                                })
                                fileList.selection.select(lowIndex,
                                                          styleData.row)
                            } else {
                                deselectAll()
                                pressToElement(styleData.row)
                                fileList.selection.select(styleData.row)
                            }
                        } else if (mouse.button === Qt.RightButton) {
                            deselectAll()
                            pressToElement(styleData.row)
                            fileList.selection.select(styleData.row)
                            _itemDelegatePopup.open()
                        }
                    }
                    onPressAndHold: {
                        inputName = false
                        console.debug("Press and hold")
                        fileList.selection.select(styleData.row)
                    }

                    onDoubleClicked: {
                        inputName = false
                        parent.forceActiveFocus()
                        if (mouse.button === Qt.LeftButton) {
                            deselectAll()
                            openFolder(styleData.row)
                        }
                    }
                }

                Popup {
                    id: _itemDelegatePopup
                    width: 100
                    height: 150
                    modal: true
                    Overlay.modal: Rectangle {
                        color: "#aacfdbe7"
                    }
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 2

                        PopupItem {
                            text: qsTr("Удалить")
                            onButtonPress: {
                                deleteSelected()
                            }
                            onButtonReleased: {
                                _itemDelegatePopup.close()
                            }
                        }
                        PopupItem {
                            text: qsTr("Переименовать")
                            onButtonPress: {
                                inputName = true
                                _txtInput.forceActiveFocus()
                                //                            deleteSelected()
                            }
                            onButtonReleased: {
                                _itemDelegatePopup.close()
                            }
                        }
                        PopupItem {

                            text: qsTr("Отправить")

                            onButtonPress: {
                                sendFiles()
                            }
                            onButtonReleased: {
                                _itemDelegatePopup.close()
                            }
                        }
                        PopupItem {
                            text: qsTr("Подписать")
                            onButtonPress: {
                                signedFiles()
                            }
                            onButtonReleased: {
                                _itemDelegatePopup.close()
                            }
                        }
                    }
                }
            }
        }
    }

    function deselectAll() {
        fileList.selection.clear()
    }
    function sendFiles() {
        if (fileList.selection.count > 0) {
            fileList.selection.forEach(function (rowIndex) {
                copyElement(rowIndex)
            })
        }
    }
    function signedFiles() {
        if (fileList.selection.count > 0) {
            fileList.selection.forEach(function (rowIndex) {
                signedElement(rowIndex)
            })
        }
    }
    function deleteSelected() {
        if (fileList.selection.count > 0) {
            fileList.selection.forEach(function (rowIndex) {
                deleteItem(rowIndex)
            })
        }
    }
    function moveElement() {}

    Keys.onPressed: {
        if (event.key === Qt.Key_Shift) {
            fileList.shiftPressed = true
        }
        if (event.key === Qt.Key_Control) {
            fileList.controlPressed = true
        }
    }
    Keys.onReleased: {
        if (event.key === Qt.Key_Shift) {
            fileList.shiftPressed = false
        }
        if (event.key === Qt.Key_Control) {
            fileList.controlPressed = false
        }

        if (event.key === Qt.Key_Delete) {
            if (inputName === false)
                deleteSelected()
        }
        if (event.key === Qt.Key_Backspace) {
            if (inputName === false)
                deleteSelected()
        }
        if (event.key === Qt.Key_F5) {
            sendFiles()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

