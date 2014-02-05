#include "connection.h"

Connection::Connection(Neuron *source, Neuron *target) :
    m_sourceNeuron(source),
    m_targetNeuron(target)
{
}

int Connection::timeLeft() const
{
    return m_timeLeft;
}

void Connection::setTimeLeft(int timeLeft)
{
    m_timeLeft = timeLeft;
}
double Connection::weight() const
{
    return m_weight;
}

void Connection::setWeight(double weight)
{
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


