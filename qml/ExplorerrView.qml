import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.3

Rectangle {
    property alias listModel: fileList.model
    signal pressToElement(int index)
    signal openFolder(int index)
    TableView {
        id: fileList

        anchors.margins: 10
        anchors.fill: parent
        model: dataModel
        clip: true

        TableViewColumn {
            role: "name"
            title: "Filename"
            width: 150
        }
        TableViewColumn {
            role: "size"
            title: "Size"
            width: 50
        }
        TableViewColumn {
            role: "date"
            title: "Date"
            width: 150
        }
        itemDelegate: Item {
            Text {
                anchors.centerIn: parent
                renderType: Text.NativeRendering
                text: styleData.value
            }
            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    openFolder(styleData.row)
                }
            }
        }
    }
}
