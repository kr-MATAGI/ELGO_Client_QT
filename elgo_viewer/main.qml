import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: mainWidnow
    width: 640
    height: 480
    visible: true
    title: qsTr("QR CODE DISPLAY")

    Rectangle{
        id: qrTextRect
        width: parent.width
        height: parent.height
        color: "white"

        Text {
            anchors.horizontalCenter: qrTextRect.horizontalCenter
            anchors.verticalCenter: qrTextRect.verticalCenter
            id: qrTextBox
            text: qsTr("QR Code 생성 중...")
            color: "black"
        }
    }

}
