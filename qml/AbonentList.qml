import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2

Dialog {
    id: popup
    modal: true
    title: "Список абонентов"
    height: 400
    width: 500

    contentItem: ListView {
        id: _listModel
        anchors.fill: parent
        anchors.topMargin: 40
        anchors.bottomMargin: 40
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
}
