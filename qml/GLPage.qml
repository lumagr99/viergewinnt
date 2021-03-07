import QtQuick 2.12
import GlComponents 1.0


Item {
    property alias vierGewinntScene: vierGewinntScene
    property alias mouseArea: mouseArea

    VierGewinntScene {
        id: vierGewinntScene
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        x: 0
        y: 0
        width: parent.width
        height: parent.height

        acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

        onClicked: {
            vierGewinntScene.mousePressed(mouse.x, mouse.y);
        }

        onPressed: {
            vierGewinntScene.mousePressed(mouse.x, mouse.y);
        }

        onReleased: {
            vierGewinntScene.mouseReleased(mouse.x, mouse.y);
        }

        onPositionChanged: {
            vierGewinntScene.mousePositionChanged(mouse.x, mouse.y);
        }

        onWheel: {
            vierGewinntScene.handleWheelEvent(wheel.angleDelta.y)
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:0;width:0}
}
##^##*/
