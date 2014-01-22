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

    Neuron {
        id: neuron1
        x: 100
        y: 100

        MouseArea {
            anchors.fill: parent
            drag.target: parent
        }
    }

    Neuron {
        id: neuron2
        x: 200
        y: 150
    }

    Neuron {
        id: neuron3
        x: 200
        y: 200
    }

    Neuron {
        id: neuron4
        x: 200
        y: 200
    }

    Connection {
        sourceNeuron: neuron1
        targetNeuron: neuron2
    }

    Connection {
        sourceNeuron: neuron1
        targetNeuron: neuron3
    }

    Connection {
        sourceNeuron: neuron1
        targetNeuron: neuron4
    }

    Connection {
        sourceNeuron: neuron4
        targetNeuron: neuron2
    }

    Connection {
        sourceNeuron: neuron4
        targetNeuron: neuron3
    }
}
