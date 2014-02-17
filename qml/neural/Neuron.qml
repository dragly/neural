import QtQuick 2.0
import NeuralNetwork 1.0

Rectangle {
    id: neuronRoot
    property NeuronAdapter neuronAdapter: null
    signal clicked

    width: 50
    height: 25
    color: neuronAdapter.isInput ? "lightgreen" : (neuronAdapter.isOutput ? "red" : "yellow")

    MouseArea {
        anchors.fill: parent
        drag.target: parent
        onClicked: {
            neuronRoot.clicked()
        }
    }
}
