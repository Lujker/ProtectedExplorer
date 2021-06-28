import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    Layout.preferredWidth: parent.width
    Layout.preferredHeight: parent.height / 4
    property bool isHovered: false
    property string text: ""
    signal buttonPress
    signal buttonReleased

    Text {
        id: _text
        text: parent.text
        color: isHovered ? "skyblue" : "black"
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            buttonPress()
        }
        onReleased: {
            buttonReleased()
        }
        onHoveredChanged: {
            parent.isHovered = !parent.isHovered
        }
    }
}
