import QtQuick 2.1
import QtQuick.Controls 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 1280
    height: 720

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    SCurve {
        color: "blue"
        startPoint: Qt.point(100, 100)
        endPoint: Qt.point(200, 200)
    }

    SCurve {
        color: "green"
        startPoint: Qt.point(100, 100)
        endPoint: Qt.point(200, 250)
    }

    SCurve {
        color: "purple"
        startPoint: Qt.point(100, 100)
        endPoint: Qt.point(200, 300)
    }
}
