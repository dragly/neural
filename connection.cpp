#include "connection.h"

#include <vector>

using namespace std;

Connection::Connection(Neuron *source, Neuron *target) :
    m_sourceNeuron(source),
    m_targetNeuron(target),
    m_previousWeight(0)
{
}

void Connection::setChanged(bool changed)
{
    m_changed = changed;
}
double Connection::weight() const
{
    return m_weight;
}

void Connection::setWeight(double weight)
{
    m_previousWeight = m_weight;
    m_weight = weight;
}

Neuron *Connection::sourceNeuron()
{
    return m_sourceNeuron;
}

Neuron *Connection::targetNeuron()
{
    return m_targetNeuron;
}

bool Connection::isChanged()
{
    return m_changed;
}

void Connection::restorePrevious() {
    m_weight = m_previousWeight;
}

