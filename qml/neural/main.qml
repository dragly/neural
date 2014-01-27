import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Particles 2.0
import NeuralNetwork 1.0

ApplicationWindow {
    id: appWindow
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

    NeuralNetworkAdapter {
        id: neuralNetwork
        anchors.fill: parent

        Component.onCompleted: {
            var nextX = 100;
            var nextY = 100;
            var neurons = []
            for(var i in neuronAdapters) {
                var neuronAdapter = neuronAdapters[i]
                // Create neurons based on neural network
                var component = Qt.createComponent("Neuron.qml");
                var properties = {
                    x: nextX,
                    y: nextY,
                    neuronAdapter: neuronAdapter
                }
                var neuron = component.createObject(neuralNetwork, properties);

                if (neuron === null) {
                    // Error Handling
                    console.log("Error creating object");
                    console.log(component.errorString())
                    continue
                }
                neuronAdapter.neuronQML = neuron

                nextX += 100
                if(nextX > width) {
                    nextY += 50
                    nextX = 50
                }
            }
            for(var i in neuronAdapters) {
                var sourceNeuronAdapter = neuronAdapters[i]
                var sourceNeuron = sourceNeuronAdapter.neuronQML
                for(var j in sourceNeuronAdapter.outputNeuronAdapters) {
                    var targetNeuronAdapter = neuronAdapters[j]
                    var targetNeuron = targetNeuronAdapter.neuronQML
                    var component = Qt.createComponent("Connection.qml");
                    var properties = {
                        sourceNeuron: sourceNeuron,
                        targetNeuron: targetNeuron
                    }
//                    console.log(sourceNeuronAdapter.neuronQML + " -> " + targetNeuronAdapter.neuronQML)
                    var connection = component.createObject(neuralNetwork, properties);
                }
            }
        }
    }


    //    ParticleSystem {
    //        id: particleSystem

    //        anchors.fill: parent

    //        Emitter {
    //           anchors.fill: parent
    //           enabled: true
    //           velocity: AngleDirection {
    //               magnitude: 100
    //               angleVariation: 10
    //           }
    //           size: 20
    //        }

    //        ItemParticle {
    //            delegate: Rectangle {
    //                width: 20
    //                height: 20
    //                color: "black"
    //            }
    //        }
    //    }
}
