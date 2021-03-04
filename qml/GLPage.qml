import QtQuick 2.12
import GlComponents 1.0

GLForm {
    id: glPage
    buttonRotateLeft.onClicked: vierGewinntScene.rotateLeft(1.0)
    buttonStop.onClicked: vierGewinntScene.stopRotation()
    buttonRotateRight.onClicked: vierGewinntScene.rotateRight(1.0)
    mouseArea.acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

    mouseArea.onClicked: {
        vierGewinntScene.mousePressed(mouse.x, mouse.y);
    }

    mouseArea.onPressed: {
        vierGewinntScene.mousePressed(mouse.x, mouse.y);
    }

    mouseArea.onReleased: {
        vierGewinntScene.mouseReleased(mouse.x, mouse.y);
    }

    mouseArea.onPositionChanged: {
        vierGewinntScene.mousePositionChanged(mouse.x, mouse.y);
    }

    mouseArea.onWheel: {
        vierGewinntScene.handleWheelEvent(wheel.angleDelta.y)
    }

    Keys.onPressed: {
        vierGewinntScene.handleKeyEvent(event.key);
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:0;width:0}
}
##^##*/
