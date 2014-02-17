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
    m_isInput = neuron->isInput();
    emit isInputChanged(m_isInput);
    m_isOutput = neuron->isOutput();
    emit isOutputChanged(m_isOutput);
}

void NeuronAdapter::addOutputNeuronAdapter(NeuronAdapter *neuronAdapter)
{
    m_outputNeuronAdapters.append(neuronAdapter);
}

