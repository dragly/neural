#include "connection.h"

#include <vector>

using namespace std;

Connection::Connection(Neuron *source, Neuron *target) :
    m_sourceNeuron(source),
    m_targetNeuron(target),
    m_previousWeight(0),
    m_previousBetter(false)
{
}

int Connection::lifeTime() const
{
    return m_lifeTime;
}

void Connection::incrementLifeTime() {
    m_lifeTime += 1;
}

void Connection::setPreviousBetter(bool isPreviousBetter)
{
    m_previousBetter = isPreviousBetter;
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

bool Connection::isPreviousBetter() {
    return m_previousBetter;
}

void Connection::restorePrevious() {
    m_previousBetter = false;
    swap(m_weight, m_previousWeight);
}

