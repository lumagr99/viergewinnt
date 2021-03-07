import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: applicationWindow
    width: 1024
    height: 768
    visible: true
    title: qsTr("Vier Gewinnt")

    GLPage {
        id: glPage
        focus: true
        anchors.fill: parent
        vierGewinntScene.viewportY: applicationWindow.height - mapToItem(applicationWindow.contentItem, x, y + height).y
        vierGewinntScene.onGameOver: {
            vierGewinntScene.startRotation(1.0);
            gameOverDialog.winnerMessage = message
            gameOverDialog.visible = true
        }
    }

    GameOverDialog {
        id: gameOverDialog
        visible: false
    }
}
