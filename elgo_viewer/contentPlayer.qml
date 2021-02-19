import QtQuick 2.12
import QRCode 1.0

Item {
    width: 600
    height: 600
    objectName: "QRItem"

    QRMaker {
        anchors.fill: parent
    }
}
