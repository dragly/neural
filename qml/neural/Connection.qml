import QtQuick 2.0

Item {
    property Neuron sourceNeuron
    property Neuron targetNeuron
    SCurve {
        id: sCurve

        visible: startPoint.x < endPoint.x

        startPoint: Qt.point(sourceNeuron.x + sourceNeuron.width, sourceNeuron.y + sourceNeuron.height / 2)
        endPoint: Qt.point(targetNeuron.x, targetNeuron.y + targetNeuron.height / 2)
    }
    BackCurve {
        id: backCurve

        startPoint: Qt.point(sourceNeuron.x + sourceNeuron.width, sourceNeuron.y + sourceNeuron.height / 2)
        endPoint: Qt.point(targetNeuron.x, targetNeuron.y + targetNeuron.height / 2)

        visible: startPoint.x >= endPoint.x

    }
}
