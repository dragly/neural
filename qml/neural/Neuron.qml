import QtQuick 2.0

Rectangle {
    width: 50
    height: 25
    color: "lightgreen"

    MouseArea {
        anchors.fill: parent
        drag.target: parent
    }
}
