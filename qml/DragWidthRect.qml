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
                if (newX > mainWindow.width - 20)
                    parent.x = mainWindow.width - 20
                else if (newX < 20)
                    parent.x = 20
                else
                    parent.x = newX
            }
        }
    }
}
