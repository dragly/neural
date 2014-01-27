#include "neuronadapter.h"

NeuronAdapter::NeuronAdapter(QQuickItem *parent) :
    QQuickItem(parent),
    m_name("banana")
{
}

Neuron *NeuronAdapter::neuron() const
{
    return m_neuron;
}

void NeuronAdapter::setNeuron(Neuron *neuron)
{
    m_neuron = neuron;
}

void NeuronAdapter::addOutputNeuronAdapter(NeuronAdapter *neuronAdapter)
{
    m_outputNeuronAdapters.append(neuronAdapter);
}

