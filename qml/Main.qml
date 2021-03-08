import QtQuick 2.12
import QtQuick.Controls 2.12
import GlComponents 1.0


ApplicationWindow {
    id: applicationWindow
    width: 800
    height: 600
    visible: true
    title: qsTr("Connect Four")

    menuBar: MenuBar {
        id: menuBar

        Menu {
            title: qsTr("Game")

            MenuItem {
                id: startAction
                text: qsTr("&Start Game")
                onTriggered: {
                    vierGewinntScene.startGame();
                }
            }

            MenuItem {
                id: pauseAction
                text: qsTr("&Stop Game")
                onTriggered: {
                    vierGewinntScene.stopGame();
                }
            }

            MenuItem {
                id: newAction
                text: qsTr("&New Game")
                onTriggered: {
                    vierGewinntScene.stopGame();
                    vierGewinntScene.newGame();
                    vierGewinntScene.startGame();
                }
            }

            MenuSeparator {
            }

            MenuItem {
                id: closeAction
                text: qsTr("&Exit")
                onTriggered: Qt.quit();
            }
        }

        Menu {
            title: qsTr("Debug Mode")

            MenuItem{
                id: showAxesAction
                text: qsTr("Show coordinate Axes")
                checkable: true
                onTriggered: {
                    vierGewinntScene.toggleAxes()
                }
            }

            MenuItem{
                id: showMouseRaysAction
                text: qsTr("Show Mouse Rays")
                checkable: true
                onTriggered: {
                    vierGewinntScene.toggleMouseRays()
                }
            }
        }
    }

    VierGewinntScene {
        id: vierGewinntScene
        anchors.fill: parent

        MouseArea {
            id: mouseArea
            anchors.fill: parent

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

        onGameOver: {
            vierGewinntScene.startRotation(1.0);
            if(color == "Red") {
                gameOverDialog.winnerMessage = qsTr("Red") + " " + qsTr("has won!")
                gameOverDialog.winnerColor = "red"
            } else if (color == "Green"){
                gameOverDialog.winnerMessage = qsTr("Green") + " " + qsTr("has won!")
                gameOverDialog.winnerColor = "green"
            } else {
                gameOverDialog.winnerMessage = qsTr("Draw!")
            }
            gameOverDialog.visible = true
        }
    }

    GameOverDialog {
        id: gameOverDialog
        visible: false
    }
}
