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

/*
            Image {
                id: elgoLgo
                width: (parent.width) / 5
                height: (parent.height) / 5
                anchors.horizontalCenter: parent.horizontalCenter
                source: "/test/images/cat_1.png"
            }

            Text {
                id: qrTextBox
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("QR Code 생성 중...")
                FontLoader {
                    id:cutsomFont
                    name:"Courier"
                }

                color: "black"
            }



            Image {
                width: (parent.width) /2
                height: (parent.height) / 2
                anchors.horizontalCenter: parent.horizontalCenter
                id: qrImagea
                source: "/test/images/apple.png"
            }


            Loader {
                id: testPageLoader
                anchors.top: parent.bottom
                onStatusChanged: {
                    console.log(testPageLoader.status);
                }
            }

            MouseArea {
            anchors.fill: parent
            onClicked: {
                    testPageLoader.source = "contentPlayer.qml"
                }
            }
*/
        }
    }

}
