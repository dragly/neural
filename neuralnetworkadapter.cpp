#include "neuralnetworkadapter.h"

#include <connection.h>

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlProperty>

NeuralNetworkAdapter::NeuralNetworkAdapter(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_neuralNetwork.setup(7, 2, 1, 5);

    for(Neuron* neuron : m_neuralNetwork.neurons()) {
        NeuronAdapter* neuronAdapter = new NeuronAdapter(this);
        neuronAdapter->setNeuron(neuron);
        m_neuronAdapters.append(neuronAdapter);
    }

    for(NeuronAdapter* neuronAdapter : m_neuronAdapters) {
        Neuron* neuron = neuronAdapter->neuron();
        for(uint i = 0; i < neuron->outputConnections().size(); i++) {
            Connection* outputConnection = neuron->outputConnections().at(i);
            Neuron* outputNeuron = outputConnection->targetNeuron();
            for(NeuronAdapter* testAdapter : m_neuronAdapters) {
                if(testAdapter->neuron() == outputNeuron) {
                    neuronAdapter->addOutputNeuronAdapter(testAdapter);
                }
            }
        }
    }
}

void NeuralNetworkAdapter::componentComplete() {
//    QQmlEngine *engine = qmlEngine(this);
//    QQmlComponent component(engine, QUrl::fromLocalFile("qml/neural/Neuron.qml"));
//    QObject *object = component.create();
//    qDebug() << object;
//    QQmlProperty::write(object, "parent", QVariant::fromValue<QObject*>(this));
//    object->setParent(this);
//    qDebug() << qmlContext(this)->engine();

    QQuickItem::componentComplete();
}
