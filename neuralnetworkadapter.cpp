#include "neuralnetworkadapter.h"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlProperty>

NeuralNetworkAdapter::NeuralNetworkAdapter(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_neuralNetwork.setup(20, 2, 1, 5);

    for(Neuron* neuron : m_neuralNetwork.neurons()) {
        NeuronAdapter* neuronAdapter = new NeuronAdapter(this);
        neuronAdapter->setNeuron(neuron);
        m_neuronAdapters.append(neuronAdapter);
    }

    for(NeuronAdapter* neuronAdapter : m_neuronAdapters) {
        Neuron* neuron = neuronAdapter->neuron();
        for(int i = 0; i < neuron->outputNeurons().size(); i++) {
            Neuron* outputNeuron = neuron->outputNeurons().at(i);
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
