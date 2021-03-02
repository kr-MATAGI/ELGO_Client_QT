import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    id: mainWidnow
    width: (Screen.width) / 3
    height: (Screen.height) / 2
    visible: true
    title: qsTr("QR CODE DISPLAY")    

    Rectangle{
        id: qrTextRect
        width: parent.width
        height: parent.height
        radius: 20
        gradient: Gradient {
            GradientStop { position: 0.0; color: "red" }
            GradientStop { position: 0.33; color: "yellow" }
            GradientStop { position: 1.0; color: "green" }
        }

        PropertyAnimation {
            target: qrTextRect
            properties: "radius"
            from: 0; to: 20; duration: 3000
            running: true
        }

        ColumnLayout {
            id: mainLayout
            width: parent.width
            height: parent.height
        }
    }
}
