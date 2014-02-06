#include "connection.h"

#include <vector>

using namespace std;

Connection::Connection(Neuron *source, Neuron *target) :
    m_sourceNeuron(source),
    m_targetNeuron(target),
    m_previousWeight(0)
{
}

int Connection::lifeTime() const
{
    return m_lifeTime;
}

void Connection::incrementLifeTime() {
    m_lifeTime += 1;
}

void Connection::setChanged(bool changed)
{
    m_changed = changed;
}

void Connection::setLifeTime(int lifeTime)
{
    m_lifeTime = lifeTime;
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

