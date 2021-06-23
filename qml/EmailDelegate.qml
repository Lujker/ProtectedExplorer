import QtQuick 2.0

Rectangle {
    id: contentItem
    property bool isHovered: false
    property bool isSelected: false
    property alias image: _icon.source
    property alias senderName: _txtName.text
    property alias mesTitle: _txtTitle.text
    property alias dateTxt: _txtDate.text
    border.color: "black"
    border.width: 1
    color: isHovered ? (isSelected ? "lightsteelblue" : "skyblue") : (isSelected ? "skyblue" : "white")
    height: 32

    Item {
        id: _mesIcon
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 48
        Image {
            id: _icon
            height: 24
            width: 24
            anchors.centerIn: parent
            source: "qrc:/../icons/contacts.png"
        }
    }
    Column {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: _mesIcon.right
        Text {
            id: _txtName
            verticalAlignment: Text.AlignVCenter
            text: "Отправитель"
            font.bold: true
        }
        Text {
            id: _txtTitle
            verticalAlignment: Text.AlignVCenter
            text: "Заголовок"
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
            text: "Дата"
            font.bold: true
        }
    }
}
