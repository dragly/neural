#include "neuron.h"

#include <connection.h>

#include <iostream>
#include <cmath>

using namespace std;

Neuron::Neuron() :
    m_isInput(false),
    m_isOutput(false),
    m_inputValue(0),
    m_outputValue(0),
    m_addition(0)
{
}

void Neuron::reset() {
    if(!m_isInput) {
        m_inputValue = 0;
        m_inputValueSum = 0;
        m_outputValue = 0;
        m_nInputValues = 0;
    }
}

int Neuron::id()
{
    return m_id;
}

void Neuron::setID(int id)
{
    m_id = id;
}

void Neuron::setAddition(double addition)
{
    m_previousAddition = m_addition;
    m_addition = addition;
}

double Neuron::addition() {
    return m_addition;
}
bool Neuron::isChanged() const
{
    return m_changed;
}

void Neuron::setChanged(bool changed)
{
    m_changed = changed;
}

void Neuron::restorePrevious()
{
    m_addition = m_previousAddition;
}


void Neuron::setInputValue(double value) {
    if(!m_isInput) {
        cerr << "Setting input value on a neuron that is not an input neuron" << endl;
    }
    m_inputValue = value;
    advance();
}

void Neuron::addInputValue(double value)
{
    m_inputValueSum += value;
    m_nInputValues += 1;
}

void Neuron::advance()
{
    if(m_isInput) {
        m_outputValue = m_inputValue;
    } else {
        if(m_nInputValues > 0) {
            m_inputValue = m_inputValueSum;
        } else {
            m_inputValue = 0;
        }
        m_outputValue = 1 / (1 + exp(-(m_inputValue + m_addition))) * 2 - 1;
//        m_outputValue = sin(m_inputValue + m_addition);
    }
//    m_outputValue = m_inputValue;
    m_inputValueSum = 0;
    m_nInputValues = 0;
}

void Neuron::sendOutput()
{
    for(Connection* connection : m_outputConnections) {
        Neuron* otherNeuron = connection->targetNeuron();
//        cout << "From " << id() << " to " << otherNeuron->id() << " " << connection->weight() << "*" << m_outputValue << endl;
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
