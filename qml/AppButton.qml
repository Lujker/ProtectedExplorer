import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: myButton
    border.color: "black"
    color: "skyblue"
    signal buttonClicked
    property alias text: txt.text
    property bool isHovered: false
    property bool isClicked: false
    width: 82
    height: 36
    radius: 12
    opacity: myButton.isHovered ? 1 : 0.3

    MouseArea {
        id: mAreaa
        anchors.fill: parent
        hoverEnabled: true
        onHoveredChanged: {
            myButton.isHovered = !myButton.isHovered
        }
        onClicked: {

            myButton.buttonClicked()
        }
        onPressed: {
            myButton.border.color = "green"
        }
        onReleased: {
            myButton.border.color = "black"
        }
    }
    Text {
        id: txt
        anchors.centerIn: parent
        text: qsTr("text")
    }
}
