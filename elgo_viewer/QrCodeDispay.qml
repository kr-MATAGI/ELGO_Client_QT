import QtQuick 2.12
import QtQuick.Window 2.12
import QRCode 1.0

Window {
    width: 600
    height: 600
    objectName: "QRItem"

    QRMaker {
        anchors.fill: parent
    }
}
