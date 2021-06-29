import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2

Dialog {
    id: popup
    modal: true
    title: "Список абонентов"
    height: 400
    width: 500

    contentItem: Rectangle {
        anchors.fill: parent
        ListView {
            id: _listModel
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.bottomMargin: 100
            model: AbonentModel
            delegate: Component {
                AbonentPopupItem {
                    width: popup.width
                    image: icon
                    abName: name
                    inboxTxt: from
                    outboxTxt: to
                }
            }
        }
        AppButton {
            text: "Добавить"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 40
            anchors.bottomMargin: 20
            onButtonClicked: {
                _addAbobnentDialog.open()
            }
        }
        AppButton {
            text: "Сохранить"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 40
            anchors.bottomMargin: 20
            onButtonClicked: {
                AppSettings.save_app_settings()
                popup.close()
            }
        }
    }

    AddAbonentDialog{
        id: _addAbobnentDialog
    }
}
