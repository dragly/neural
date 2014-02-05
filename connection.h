#ifndef CONNECTION_H
#define CONNECTION_H

class Neuron;
class Connection
{
public:
    Connection(Neuron* source, Neuron* target);

    int timeLeft() const;
    void setTimeLeft(int timeLeft);

    double weight() const;
    void setWeight(double weight);

    Neuron* sourceNeuron();
    Neuron* targetNeuron();

private:
    Neuron* m_sourceNeuron;
    Neuron* m_targetNeuron;
    int m_timeLeft;
    double m_weight;
};

#endif // CONNECTION_H
