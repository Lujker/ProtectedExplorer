import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Rectangle {
    id: contentItem
    property bool isHovered: false
    property bool isSelected: false
    property alias image: _icon.source
    property alias abName: _txtName.text
    property alias outboxTxt: _abOubox.text
    property alias inboxTxt: _abInbox.text
    border.color: "black"
    border.width: 1
    color: isHovered ? (isSelected ? "lightsteelblue" : "skyblue") : (isSelected ? "skyblue" : "white")
    height: 32
    width: parent.width
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button === Qt.RightButton) {
                _listPopup.open()
            }
            if (mouse.button === Qt.LeftButton) {
                //                isSelected = !isSelected
            }
        }
    }
    Item {
        id: _abIcon
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 24
        Image {
            id: _icon
            property bool iconHovered: false
            height: 24
            width: 24
            anchors.centerIn: parent
            source: "qrc:/../icons/contacts.png"
            rotation: iconHovered ? 20 : 0
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    parent.iconHovered = !parent.iconHovered
                }
                onClicked: {
                    _dialogForIcon.index = index
                    _dialogForIcon.open()
                }
            }
        }
    }
    Item {
        id: _abName
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: _abIcon.right
        width: 100
        Text {
            id: _txtName
            property bool nameHovered: false
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            text: "Имя"
            font.bold: true
            color: nameHovered ? "green" : "black"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    parent.nameHovered = !parent.nameHovered
                }
                onClicked: {
                    isSelected = true
                    _dialogNewName.index = index
                    _dialogNewName.open()
                }
            }
        }
    }
    Column {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 5
        Text {
            id: _abOubox
            property bool outboxHovered: false
            verticalAlignment: Text.AlignVCenter
            text: "Исходящие"

            //            font.bold: true
            color: outboxHovered ? "green" : "black"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    parent.outboxHovered = !parent.outboxHovered
                }
                onClicked: {
                    isSelected = true
                    _dialogForOutboxes.index = index
                    _dialogForOutboxes.open()
                }
            }
        }
        Text {
            id: _abInbox
            property bool inboxHovered: false
            verticalAlignment: Text.AlignVCenter
            text: "Входящие"
            color: inboxHovered ? "green" : "black"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    parent.inboxHovered = !parent.inboxHovered
                }
                onClicked: {
                    isSelected = true
                    _dialogForInboxes.index = index
                    _dialogForInboxes.open()
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
            AbonentModel.setInPath(index, fileUrl)
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
            AbonentModel.setOutPath(index, fileUrl)
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }
    Dialog {
        id: _dialogNewName
        title: "Введите имя для абонента"
        height: 100
        width: 200
        property int index: -1

        TextField {
            id: _txtFieldAbName
            anchors.fill: parent
            anchors.centerIn: parent.Center
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            AbonentModel.renameAbonent(index, _txtFieldAbName.text)
        }
        onRejected: {

        }
    }
    FileDialog {
        id: _dialogForIcon
        title: "Выбирите путь к иконке абонента"
        folder: shortcuts.home
        nameFilters: ["Image files (*.jpg *.png)"]
        property int index: -1
        onAccepted: {
            AbonentModel.setIconPath(index, fileUrl)
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }
    Popup {
        id: _listPopup
        width: 150
        height: 100
        dim: true
        modal: true
        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
            }
        }
        exit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
            }
        }
        Overlay.modal: Rectangle {
            color: "#aacfdbe7"
        }
        ColumnLayout {
            anchors.fill: parent
            spacing: 2

            PopupFMItem {
                text: qsTr("Добавить абоннета")
                onButtonPress: {
                    AbonentModel.addAbonent()
                }
                onButtonReleased: {
                    _listPopup.close()
                }
            }
            PopupFMItem {
                text: qsTr("Удалить")
                onButtonPress: {
                    AbonentModel.delAbonent(index)
                }
                onButtonReleased: {
                    _listPopup.close()
                }
            }
        }
    }
}
