import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Popup {
    id: _listPopup
    width: 100
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
        ///!\todo Сделать диалог по найтройке нового абонента
        ///куда будет всписываться инфа о нем и уже эту инфу передает в модель
        PopupFMItem {
            text: qsTr("Новый абоннет")
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
