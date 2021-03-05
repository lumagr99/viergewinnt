import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: applicationWindow
    width: 1024
    height: 768
    visible: true
    title: qsTr("Vier Gewinnt")

    SwipeView {
        id: swipeView
        focus: true
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        Component.onCompleted: contentItem.interactive = false

        GLPage {
            vierGewinntScene.viewportY: applicationWindow.height - mapToItem(applicationWindow.contentItem, x, y + height).y;
        }

        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }
}
