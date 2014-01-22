#include "neuron.h"

Neuron::Neuron()
{
}

void Neuron::setInputValue(double value) {
    m_inputValue = value;
}

void Neuron::addInputValue(double value)
{
    m_inputValue += value;
}

void Neuron::advance()
{
    m_outputValue = m_inputValue;
    m_inputValue = 0;
}

void Neuron::sendOutput()
{
    for(Neuron* neuron : m_outputNeurons) {
        neuron->addInputValue(m_outputValue);
    }
}

void Neuron::addOutputNeuron(Neuron *other)
{
    m_outputNeurons.push_back(other);
}

double Neuron::outputValue() {
    return m_outputValue;
}
