import QtQuick 2.12
import QtQuick.Controls 2.5

Dialog {
    visible: true
    title: qsTr("Game over!")
    modal: false
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnReleaseOutsideParent
    standardButtons: Dialog.Yes | Dialog.No

    property alias winnerMessage : winnerMessage.text
    property alias winnerColor: winnerMessage.color

    Rectangle {
        color: "lightgray"
        implicitWidth: 400
        implicitHeight: 130

        Text {
            id: winnerMessage
            font.pixelSize: 24
            text: ""
            topPadding: 30
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: playAgainMessage
            font.pixelSize: 18
            text: "Möchten Sie eine weitere Runde spielen?"
            topPadding: 50
            anchors.centerIn: parent
        }
    }

    onAccepted: console.log("JAAAAA")
    onRejected: Qt.quit()
}
