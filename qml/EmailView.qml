import QtQml 2.15
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Rectangle {
    id: explWindow
    property alias listModel: messageList.model ///переменная для установки модэли для отображения
    property bool focusOfView: messageList.focus
    property bool inputName: false ///проиcходит ввод нового имения для файлы

    ///Сигналы:
    ///Выбор элемента по щелчку
    signal selectElement(int index)
    ///сигнал при перетаскивании элемента
    signal strartDragElem
    ///сигнал при завершении перетаскивания
    signal endDragElem
    ///Расположения
    anchors.topMargin: 10
    anchors.bottomMargin: 10
    border.width: 1

    Component {
        id: headerItem
        EmailDelegate {
            width: explWindow.width
        }
    }

    Component {
        id: contentDeleagte
        EmailDelegate {
            width: explWindow.width
            image: icon
            senderName: sender
            mesTitle: title
            dateTxt: date
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onHoveredChanged: {
                    parent.isHovered = !parent.isHovered
                }
                onClicked: {
                    parent.isSelected = !parent.isSelected
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
        header: headerItem
        delegate: contentDeleagte
        headerPositioning: ListView.OverlayHeader
    }
}
