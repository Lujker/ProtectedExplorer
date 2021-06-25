import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2

Popup {
    id: popup

    //    contentHeight: _listModel.contentHeight
    //    contentWidth: _listModel.contentWidth
    modal: true
    //    Overlay.modal: Rectangle {
    //        anchors.fill: parent
    //        color: "#aacfdbe7"
    //    }
    contentItem: ListView {
        id: _listModel
        implicitWidth: 300
        implicitHeight: 300
        model: AbonentModel
        anchors.fill: parent

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
