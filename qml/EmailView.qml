import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.15

Rectangle {
    id: explWindow
    property alias listModel: messageList.model ///переменная для установки модэли для отображения
    property bool focusOfView: messageList.focus
    property bool inputName: false ///проиcходит ввод нового имения для файлы
    ///Сигналы
    ///сигнал при нажатии на элемент
    signal pressToElement(int index)
    ///копирование элемента
    signal copyElements(int start, int end)
    signal copyElement(int index)
    ///удаление элементов
    signal deleteItems(int start, int end)
    signal deleteItem(int index)
    ///подпись элементов
    signal signedElements(int start, int end)
    signal signedElement(int index)
    ///перемещение файлов
    signal moveItems(int start, int end)
    signal moveItem(int index)
    ///открыте папки
    signal openFolder(int index)
    ///переименование файла под индексом
    signal renameFile(int index, string newName)
    ///нажатие на модель
    signal presToTable
    ///добавить файл
    signal addNewFile(string name)
    ///добавить папку
    signal addNewFolder(string name)
    ///сигнал при перетаскивании элемента
    signal strartDragElem
    ///сигнал при завершении перетаскивания
    signal endDragElem
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    border.width: 1

    Component {
        id: contentDeleagte
        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle {
                id: _mesIcon
                border.width: 1
                border.color: "black"
                radius: 10
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 10
                anchors.left: parent.left
                anchors.leftMargin: 5
                Text {
                    anchors.fill: parent
                    text: qsTr("lol")
                    //                    font: {
                    //                        pointSize: Math.min(parent.width, parent.height) / 3
                    //                        bold: true
                    //                    }
                }
            }
            Rectangle {
                id: _mesTitle
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2
                anchors.left: _mesIcon.right
                Text {
                    anchors.fill: parent
                    text: qsTr("text")
                }
            }
            Rectangle {
                id: _mesDate
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 10
                anchors.right: parent.right
                Text {
                    anchors.fill: parent
                    text: qsTr("text")
                }
            }
        }
    }

    ListView {
        id: messageList
        parent: explWindow
        anchors.fill: parent
        model: dataModel
        focus: true
        clip: true ///разрешить перенос текста
        property bool shiftPressed: false
        property bool controlPressed: false
        highlight: Rectangle {
            color: "lightsteelblue"
            radius: 5
        }

        delegate: contentDeleagte
    }

    //        TableViewColumn {
    //            role: "filepath"
    //            title: "Иконка"
    //            delegate: Component {
    //                id: imageDelegate
    //                Image {
    //                    anchors.verticalCenter: parent.verticalCenter
    //                    anchors.horizontalCenter: parent.horizontalCenter
    //                    fillMode: Image.PreserveAspectFit
    //                    cache: true
    //                    asynchronous: true
    //                    sourceSize.width: 8
    //                    sourceSize.height: 8
    //                    ///путь к инокне берет из проводника иконок из С++ кода
    //                    source: styleData.value ? "image://iconProvider/" + styleData.value : ""
    //                }
    //            }
    //            width: 50
    //        }
    //        TableViewColumn {
    //            role: "name"
    //            title: "Имя"
    //            //            width: explWindow.width - 341
    //            width: 180
    //        }
    //        TableViewColumn {
    //            role: "suffix"
    //            title: "Тип"
    //            width: 50
    //        }
    //        TableViewColumn {
    //            role: "size"
    //            title: "Размер"
    //            width: 70
    //        }
    //        TableViewColumn {
    //            role: "date"
    //            title: "Дата"
    //            width: 150
    //        }
    //        ///Устанока сортировки и индикатора сортировки
    //        sortIndicatorVisible: true
    //        onSortIndicatorColumnChanged: {
    //            listModel.setSorting(sortIndicatorColumn, 0)
    //        }
    //        onSortIndicatorOrderChanged: {
    //            listModel.setSorting(sortIndicatorColumn, sortIndicatorOrder)
    //        }
    //        DropArea {
    //            id: _droped
    //            anchors.fill: parent
    //            onDropped: {
    //                endDragElem()
    //            }
    //        }
    //        rowDelegate: Rectangle {
    //            id: _rowDelegate
    //            color: styleData.selected ? "skyblue" : "white"

    //            MouseArea {
    //                id: _dragArea
    //                pressAndHoldInterval: 100
    //                anchors.fill: parent
    //                acceptedButtons: Qt.LeftButton | Qt.RightButton
    //                ///При нажатии на элемент мы устанавливем фокус и в зависимости от
    //                ///нажатой кнопку мы либо выбираем элемент либо открываем контекстное меню для файла
    //                ///Если нажаты кнопки Cntrl или Shift то элементы добавляются к уже выбранными
    //                ///иначе выбирается только последний
    //                onClicked: {
    //                    fileList.focus = true
    //                    inputName = false
    //                    parent.forceActiveFocus()
    //                    if (mouse.button === Qt.LeftButton) {
    //                        if (fileList.controlPressed) {
    //                            fileList.selection.select(styleData.row)
    //                        } else if (fileList.shiftPressed) {
    //                            var lowIndex = fileList.rowCount
    //                            fileList.selection.forEach(function (rowIndex) {
    //                                if (lowIndex > rowIndex)
    //                                    lowIndex = rowIndex
    //                            })
    //                            fileList.selection.select(lowIndex, styleData.row)
    //                        } else {
    //                            deselectAll()
    //                            pressToElement(styleData.row)
    //                            fileList.selection.select(styleData.row)
    //                        }
    //                    } else if (mouse.button === Qt.RightButton) {
    //                        deselectAll()
    //                        pressToElement(styleData.row)
    //                        fileList.selection.select(styleData.row)
    //                        _itemDelegatePopup.open()
    //                    }
    //                }
    //                ///Перетаскивание вызывает сигнал начала Drag события передающееся в main, где
    //                ///затем создается иконка перетаскивания
    //                onPressAndHold: {
    //                    //                    deselectAll()
    //                    if (mouse.button === Qt.LeftButton) {
    //                        inputName = false
    //                        fileList.selection.select(styleData.row)
    //                        strartDragElem()
    //                    }
    //                }
    //                ///При двойном нажатии открыте файла или директории
    //                onDoubleClicked: {
    //                    if (mouse.button === Qt.LeftButton) {
    //                        deselectAll()
    //                        openFolder(styleData.row)
    //                    }
    //                }
    //            }
    //        }

    //        itemDelegate: Component {
    //            Item {
    //                id: fileDelegate
    //                clip: true
    //                TextInput {
    //                    id: _txtInput
    //                    anchors.left: parent.left
    //                    readOnly: styleData.column !== 1
    //                    text: styleData.value ///styleData - получение значения из С++
    //                    renderType: Text.NativeRendering
    //                    onAccepted: {
    //                        inputName = false
    //                        listModel.renameFile(styleData.row, text)
    //                        parent.forceActiveFocus()
    //                        console.debug(text)
    //                        console.debug(styleData.row)
    //                    }
    //                }

    //                ///Действие при нажатии как и у строки
    //                MouseArea {
    //                    id: _dragArea
    //                    anchors.fill: parent
    //                    pressAndHoldInterval: 100
    //                    acceptedButtons: Qt.LeftButton | Qt.RightButton
    //                    onPressed: {
    //                        inputName = false
    //                        parent.forceActiveFocus()
    //                        fileList.focus = true
    //                        if (mouse.button === Qt.LeftButton) {
    //                            if (fileList.controlPressed) {
    //                                fileList.selection.select(styleData.row)
    //                            } else if (fileList.shiftPressed) {
    //                                var lowIndex = fileList.rowCount
    //                                fileList.selection.forEach(function (rowIndex) {
    //                                    if (lowIndex > rowIndex)
    //                                        lowIndex = rowIndex
    //                                })
    //                                fileList.selection.select(lowIndex,
    //                                                          styleData.row)
    //                            } else {
    //                                deselectAll()
    //                                pressToElement(styleData.row)
    //                                fileList.selection.select(styleData.row)
    //                            }
    //                        } else if (mouse.button === Qt.RightButton) {
    //                            deselectAll()
    //                            pressToElement(styleData.row)
    //                            fileList.selection.select(styleData.row)
    //                            _itemDelegatePopup.open()
    //                        }
    //                    }

    //                    onPressAndHold: {
    //                        //                        deselectAll()
    //                        if (mouse.button === Qt.LeftButton) {
    //                            inputName = false
    //                            fileList.selection.select(styleData.row)
    //                            strartDragElem()
    //                        }
    //                    }
    //                    onDoubleClicked: {
    //                        inputName = false
    //                        parent.forceActiveFocus()
    //                        if (mouse.button === Qt.LeftButton) {
    //                            deselectAll()
    //                            openFolder(styleData.row)
    //                        }
    //                    }
    //                }

    //                ///Всплывающие подсказки
    //                Popup {
    //                    id: _itemDelegatePopup
    //                    width: 100
    //                    height: 150
    //                    modal: true
    //                    Overlay.modal: Rectangle {
    //                        color: "#aacfdbe7"
    //                    }
    //                    ColumnLayout {
    //                        anchors.fill: parent
    //                        spacing: 2

    //                        PopupFMItem {
    //                            text: qsTr("Удалить")
    //                            onButtonPress: {
    //                                deleteSelected()
    //                            }
    //                            onButtonReleased: {
    //                                _itemDelegatePopup.close()
    //                            }
    //                        }
    //                        PopupFMItem {
    //                            text: qsTr("Переименовать")
    //                            onButtonPress: {
    //                                inputName = true
    //                                _txtInput.forceActiveFocus()
    //                                //                            deleteSelected()
    //                            }
    //                            onButtonReleased: {
    //                                _itemDelegatePopup.close()
    //                            }
    //                        }
    //                        PopupFMItem {

    //                            text: qsTr("Отправить")

    //                            onButtonPress: {
    //                                sendFiles()
    //                            }
    //                            onButtonReleased: {
    //                                _itemDelegatePopup.close()
    //                            }
    //                        }
    //                        PopupFMItem {
    //                            text: qsTr("Подписать")
    //                            onButtonPress: {
    //                                signedFiles()
    //                            }
    //                            onButtonReleased: {
    //                                _itemDelegatePopup.close()
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }

    ///Функции для работы с файлами
    //    function deselectAll() {
    //        fileList.selection.clear()
    //    }
    //    function sendFiles() {
    //        if (fileList.selection.count > 0) {
    //            fileList.selection.forEach(function (rowIndex) {
    //                copyElement(rowIndex)
    //            })
    //        }
    //    }
    //    function signedFiles() {
    //        if (fileList.selection.count > 0) {
    //            fileList.selection.forEach(function (rowIndex) {
    //                signedElement(rowIndex)
    //            })
    //        }
    //    }
    //    function deleteSelected() {
    //        if (fileList.selection.count > 0) {
    //            fileList.selection.forEach(function (rowIndex) {
    //                deleteItem(rowIndex)
    //            })
    //        }
    //    }
    //    function moveElement() {
    //        if (fileList.selection.count > 0) {
    //            fileList.selection.forEach(function (rowIndex) {
    //                moveItem(rowIndex)
    //            })
    //        }
    //    }
    //    ///Обработка нажатия клавиш
    //    Keys.onPressed: {
    //        if (event.key === Qt.Key_Shift) {
    //            fileList.shiftPressed = true
    //        }
    //        if (event.key === Qt.Key_Control) {
    //            fileList.controlPressed = true
    //        }
    //    }
    //    Keys.onReleased: {
    //        if (event.key === Qt.Key_Shift) {
    //            fileList.shiftPressed = false
    //        }
    //        if (event.key === Qt.Key_Control) {
    //            fileList.controlPressed = false
    //        }

    //        if ((event.key === Qt.Key_Delete) || (event.key === Qt.Key_Backspace)) {
    //            if (inputName === false)
    //                deleteSelected()
    //        }
    //        if (event.key === Qt.Key_F5) {
    //            sendFiles()
    //        }
    //        if ((event.key === Qt.Key_Return) || (event.key === Qt.Key_Enter)) {
    //            var lastIndex
    //            if (fileList.selection.count > 0) {
    //                fileList.selection.forEach(function (rowIndex) {
    //                    lastIndex = rowIndex
    //                })
    //            }
    //            if (!inputName)
    //                openFolder(lastIndex)
    //        }
    //    }
}
