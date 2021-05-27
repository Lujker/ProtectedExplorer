import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: myButton
    border.color: "black"
    color: "skyblue"
    signal buttonClicked
    property string toolTipText: "" ///подсказку, иконку и текст устанавливаю на верхнем уровне
    property alias text: txt.text
    property alias icon: _icon.source
    property bool isHovered: false
    property bool isClicked: false
    width: 82
    height: 36
    radius: 12
    opacity: myButton.isHovered ? 1 : 0.3

    MouseArea {
        id: mAreaa
        ToolTip.text: toolTipText ///текст подсказки элемента
        ToolTip.visible: toolTipText ? (containsMouse
                                        && mainWindow.help) : false ///когда отображать подсказку
        anchors.fill: parent
        hoverEnabled: true /// разрешить перекрытие
        onHoveredChanged: {
            myButton.isHovered = !myButton.isHovered
        }
        onClicked: {

            myButton.buttonClicked()
        }
        onPressed: {
            myButton.border.color = "green" ///при нажатии границы кнопки подсвечивать зеленым
        }
        onReleased: {
            myButton.border.color = "black"
        }
    }
    ///по умолчанию кнопка без текста и иконки
    Image {
        id: _icon
        source: ""
        anchors.centerIn: parent
        height: 16
        width: 16
    }
    Text {
        id: txt
        anchors.centerIn: parent
        text: qsTr("")
    }
}
