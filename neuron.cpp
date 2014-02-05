#include "neuron.h"

#include <connection.h>

#include <iostream>

using namespace std;

Neuron::Neuron() :
    m_inputValue(0),
    m_outputValue(0),
    m_isInput(false),
    m_isOutput(false)
{
}

void Neuron::setInputValue(double value) {
    if(!m_isInput) {
        cerr << "Setting input value on a neuron that is not an input neuron" << endl;
    }
    m_inputValue = value;
}

void Neuron::addInputValue(double value)
{
    m_inputValueSum += value;
    m_nInputValues += 1;
}

void Neuron::advance()
{
    if(!m_isInput) {
        m_inputValue = m_inputValueSum / m_nInputValues;
    }
    m_outputValue = m_inputValue / m_outputConnections.size();
    m_inputValueSum = 0;
}

void Neuron::sendOutput()
{
    for(Connection* connection : m_outputConnections) {
        Neuron* otherNeuron = connection->targetNeuron();
        otherNeuron->addInputValue(connection->weight() * m_outputValue);
    }
}

void Neuron::addOutputConnection(Connection *other)
{
    m_outputConnections.push_back(other);
}

double Neuron::outputValue() {
    return m_outputValue;
}
const std::vector<Connection*>& Neuron::outputConnections() const
{
    return m_outputConnections;
}
bool Neuron::isOutput() const
{
    return m_isOutput;
}

void Neuron::setOutput(bool isOutput)
{
    m_isOutput = isOutput;
}
bool Neuron::isInput() const
{
    return m_isInput;
}

void Neuron::setInput(bool isInput)
{
    m_isInput = isInput;
}
