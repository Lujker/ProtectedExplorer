import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

Dialog {
    id: _addAbobnentDialog
    title: "Добавить нового абонента"
    standardButtons: Dialog.Ok | Dialog.Cancel
    height: 300
    width: 250
    Label {
        id: _lblName
        text: "Имя:"
        font.bold: true
        height: 20
        anchors.top: parent.Top
    }
    TextField {
        id: _sysName
        height: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _lblName.bottom
        text: "Не указано"
    }
    Label {
        id: _lblInbox
        text: "Входящие:"
        font.bold: true
        height: 20
        anchors.top: _sysName.bottom
    }
    TextField {
        id: _inbox
        height: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _lblInbox.bottom
        text: "..."
        readOnly: true
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                if (mouse.button === Qt.LeftButton) {
                    _dialogForInboxes.open()
                }
            }
        }
    }
    Label {
        id: _lblOutbox
        text: "Исходящие:"
        font.bold: true
        height: 20
        anchors.top: _inbox.bottom
    }
    TextField {
        id: _outbox
        height: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _lblOutbox.bottom
        text: "..."
        readOnly: true
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                if (mouse.button === Qt.LeftButton) {
                    _dialogForOutboxes.open()
                }
            }
        }
    }

    Label {
        id: _lblIcon
        text: "Путь к иконке:"
        font.bold: true
        height: 20
        anchors.top: _outbox.bottom
    }
    Image {
        id: iconImage
        anchors.left: _lblIcon.right
        anchors.top: _outbox.bottom
        source: _icon.text
        width: 16
        height: 16
    }
    TextField {
        id: _icon
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _lblIcon.bottom
        text: ":/../icons/contacts.png"
        readOnly: true
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                if (mouse.button === Qt.LeftButton) {
                    _dialogForIcon.open()
                }
            }
        }
    }

    FileDialog {
        id: _dialogForInboxes
        title: "Выбирите путь к входящим"
        selectFolder: true
        folder: shortcuts.home
        nameFilters: ["Inboxes"]
        property int index: -1
        onAccepted: {
            _inbox.text = fileUrl
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }
    FileDialog {
        id: _dialogForOutboxes
        title: "Выбирите путь к исходящим"
        selectFolder: true
        folder: shortcuts.home
        nameFilters: ["Outboxes"]
        property int index: -1
        onAccepted: {
            _outbox.text = fileUrl
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }

    FileDialog {
        id: _dialogForIcon
        title: "Выбирите путь к иконке абонента"
        folder: shortcuts.home
        nameFilters: ["Image files (*.jpg *.png)"]
        property int index: -1
        onAccepted: {
            _icon.text = fileUrl
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }

    onAccepted: AbonentModel.addAbonent(_sysName.text, _inbox.text,
                                        _outbox.text, _icon.text)
    onRejected: console.log("Cancel clicked")
}
