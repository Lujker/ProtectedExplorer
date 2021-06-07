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
                var newX = parent.x + mouseX
                if (newX > 1010)
                    parent.x = 1010
                else if (newX < 10)
                    parent.x = 10
                else
                    parent.x = newX
            }
        }
    }
}
