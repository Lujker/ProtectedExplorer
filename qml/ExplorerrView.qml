import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Rectangle {
    id: explWindow
    property alias listModel: fileList.model
    signal pressToElement(int index)
    signal copyElement(int start, int end)
    signal deleteItem(int start, int end)
    signal openFolder(int index)
    signal presToTable

    TableView {
        id: fileList
        anchors.margins: 10
        anchors.fill: parent
        model: dataModel
        clip: true

        property bool shiftPressed: false
        property bool controlPressed: false

        //        onClicked: {
        //            presToTable()
        //        }
        TableViewColumn {
            role: "name"
            title: "Filename"
            width: explWindow.width - 292
        }
        TableViewColumn {
            role: "suffix"
            title: "suffix"
            width: 50
        }
        TableViewColumn {
            role: "size"
            title: "Size"
            width: 70
        }
        TableViewColumn {
            role: "date"
            title: "Date"
            width: 150
        }

        //        TableViewColumn {
        //            role: "icon"
        //            title: "Image"
        //        }
        itemDelegate: Item {
            id: fileDelegate

            Text {
                anchors.left: parent.left
                renderType: Text.NativeRendering
                fontSizeMode: Text.Fit
                text: styleData.value
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    fileList.focus = true
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
                }
                onDoubleClicked: {
                    deselectAll()
                    openFolder(styleData.row)
                }
            }
        }
    }
    function deselectAll() {
        fileList.selection.clear()
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Shift) {
            console.debug("Shift pressed")
            fileList.shiftPressed = true
        }
        if (event.key === Qt.Key_Control) {
            console.debug("Control pressed")
            fileList.controlPressed = true
        }
    }
    Keys.onReleased: {
        var start
        var end
        if (event.key === Qt.Key_Shift) {
            fileList.shiftPressed = false
        }
        if (event.key === Qt.Key_Control) {
            fileList.controlPressed = false
        }
        if (event.key === Qt.Key_Delete || Qt.Key_Backspace) {
            if (fileList.selection.count > 0) {
                start = fileList.rowCount + 1
                end = 0
                fileList.selection.forEach(function (rowIndex) {
                    if (rowIndex < start)
                        start = rowIndex
                    if (rowIndex > end)
                        end = rowIndex
                })
                deleteItem(start, end)
                console.debug("delete from: ", start, " to: ", end)
            }
        }
        if (event.key === Qt.Key_F5) {
            console.debug("F5 pressed")
            if (fileList.selection.count > 0) {
                start = fileList.rowCount + 1
                end = 0
                fileList.selection.forEach(function (rowIndex) {
                    if (rowIndex < start)
                        start = rowIndex
                    if (rowIndex > end)
                        end = rowIndex
                })
                copyElement(start, end)
                console.debug("copy from: ", start, " to: ", end)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

