import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.0

Rectangle {
    id: explWindow
    property alias listModel: fileList.model
    signal pressToElement(int index)
    signal copyElements(int start, int end)
    signal copyElement(int index)
    signal deleteItems(int start, int end)
    signal deleteItem(int index)
    signal signedElements(int start, int end)
    signal signedElement(int index)
    signal openFolder(int index)
    signal presToTable
    signal addNewFile
    signal addNewFolder

    TableView {
        id: fileList
        anchors.margins: 10
        //        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: _rowLayoutTableButtom.top
        model: dataModel
        clip: true
        property bool shiftPressed: false
        property bool controlPressed: false
        property bool sortedArr: [false, false, false, false]

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
            width: 40
        }
        TableViewColumn {
            role: "name"
            title: "Имя"
            width: explWindow.width - 315
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
            console.debug(sortIndicatorColumn)
        }
        onSortIndicatorOrderChanged: {
            ///Вот тут сигнал сортировки console.debug(sortIndicatorColumn)
            console.debug(sortIndicatorOrder)
            console.debug(sortIndicatorColumn)
        }

        headerDelegate: Rectangle {
            id: _headerDelegate
            width: _textHeader.text.length * 1.2
            height: _textHeader.font.pixelSize * 1.2
            color: "skyblue"
            Text {
                id: _textHeader
                anchors.horizontalCenter: parent.horizontalCenter
                color: (styleData.pressed) ? "red" : "black"
                text: styleData.value
            }
            border {
                width: 1
                color: (styleData.pressed) ? "red" : "black"
            }
        }
        DropArea {
            anchors.fill: parent
            onEntered: {

            }
        }
        rowDelegate: Rectangle {
            id: _rowDelegate
            color: styleData.selected ? "skyblue" : "white"
        }

        itemDelegate: Component {

            Item {
                id: fileDelegate

                //                Drag.active: _dragArea.held
                //                Drag.source: _dragArea
                Text {
                    anchors.fill: parent
                    renderType: Text.NativeRendering
                    fontSizeMode: Text.Fit
                    text: styleData.value
                }

                MouseArea {
                    id: _dragArea
                    property bool held: false
                    drag.target: held ? fileDelegate : undefined
                    drag.axis: Drag.XAndYAxis
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
                        console.debug("Press and hold")
                        held = true
                    }

                    onReleased: held = false
                    onDoubleClicked: {
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
    RowLayout {
        id: _rowLayoutTableButtom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        AppButton {
            text: "Создать файл"
            toolTipText: "Создать файл в открытом каталоге таблицы"
            onButtonClicked: {
                addNewFile()
            }
        }
        AppButton {
            text: "Создать папку"
            toolTipText: "Создать папку в открытом каталоге таблицы"
            onButtonClicked: {
                addNewFolder()
            }
        }
        AppButton {
            text: "Удалить"
            toolTipText: "Удалить выбранные в таблице файлы (delete/backspace)"
            onButtonClicked: {
                deleteSelected()
            }
        }
        AppButton {
            text: "Копировать"
            toolTipText: "Копировать выбранный файлы в другую таблицу (F5)"
            onButtonClicked: {
                deleteSelected()
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
            deleteSelected()
        }
        if (event.key === Qt.Key_Backspace) {
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

