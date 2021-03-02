import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import GlComponents 1.0

Item {
    id: item1
    property alias buttonRotateLeft: buttonRotateLeft
    property alias buttonStop: buttonStop
    property alias buttonRotateRight: buttonRotateRight
    property alias vierGewinntScene: vierGewinntScene
    property alias mouseArea: mouseArea

    VierGewinntScene {
        id: vierGewinntScene
        anchors.fill: parent
        RowLayout {
            id: rowLayout
            width: 317
            height: 56
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: buttonRotateLeft
                text: qsTr("RotateLeft")
            }

            Button {
                id: buttonStop
                text: qsTr("Stop")
            }

            Button {
                id: buttonRotateRight
                text: qsTr("RotateRight")
            }
        }
    }

    MouseArea {
        id: mouseArea
        x: 0
        y: 0
        width: parent.width
        height: parent.height - rowLayout.height
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

