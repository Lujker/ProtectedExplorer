import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQml 2.15

Rectangle {
    id: explWindow
    property alias listModel: messageList.model ///переменная для установки модэли для отображения
    property bool focusOfView: messageList.focus
    property bool inputName: false ///проиcходит ввод нового имения для файлы

    ///Сигналы
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    border.width: 1

    Component {
        id: contentDeleagte

        Item {
            height: 32
            width: explWindow.visible ? parent.width : 0
            Item {
                id: _mesIcon
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: 48
                Image {
                    height: 24
                    width: 24
                    anchors.centerIn: parent
                    source: icon
                }
            }
            Column {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: _mesIcon.right
                Text {
                    id: _txtName
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr(sender)
                    font.bold: true
                }
                Text {
                    id: _txtSenderName
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr(title)
                }
            }
            Item {
                id: _mesDate
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 100
                Text {
                    id: _txtDate
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr(date)
                    font.bold: true
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

        //        highlight: Rectangle {
        //            color: "lightsteelblue"
        //            radius: 15
        //        }
        delegate: contentDeleagte
    }
}
