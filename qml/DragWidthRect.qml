import QtQuick 2.0

Rectangle {
    property bool isHovered: false
    width: 2
    MouseArea {
        cursorShape: parent.isHovered ? Qt.SplitHCursor : Qt.ArrowCursor
        anchors.fill: parent
        hoverEnabled: true /// разрешить перекрытие
        onHoveredChanged: {
            parent.isHovered = !parent.isHovered
        }
        drag {
            target: parent
            axis: Drag.XAxis
        }
        onMouseXChanged: {
            if (drag.active) {
                parent.x = parent.x + mouseX
            }
        }
    }
}
