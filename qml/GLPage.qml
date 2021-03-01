import QtQuick 2.12
import GlComponents 1.0

GLForm {
    id: glPage
    buttonRotateLeft.onClicked: myGLItem.rotateLeft(1.0)
    buttonStop.onClicked: myGLItem.stopRotation()
    buttonRotateRight.onClicked: myGLItem.rotateRight(1.0)
    mouseArea.acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
    mouseArea.onClicked: {
        console.log("Clicked, X: " + mouse.x + "Y: " + mouse.y + "Button: "+mouse.button);
        myGLItem.mousePressed(mouse.x, mouse.y);
    }
    mouseArea.onReleased: {
        console.log("Released, X: " + mouse.x + "Y: " + mouse.y + "Button: "+mouse.button);
        myGLItem.mouseReleased(mouse.x, mouse.y);
    }
    mouseArea.onMouseXChanged: {
        console.log("Moved X: X: " + mouse.x + "Y: " + mouse.y + "Button: "+mouse.button);
        myGLItem.mousePositionChanged(mouse.x, mouse.y);
    }
    mouseArea.onMouseYChanged: {
        console.log("Moved Y: X: " + mouse.x + "Y: " + mouse.y + "Button: "+mouse.button);
        myGLItem.mousePositionChanged(mouse.x, mouse.y);
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:0;width:0}
}
##^##*/
