#ifndef CONNECTION_H
#define CONNECTION_H

class Neuron;
class Connection
{
public:
    Connection(Neuron* source, Neuron* target);

    double weight() const;
    void setWeight(double weight);

    Neuron* sourceNeuron();
    Neuron* targetNeuron();

    bool isChanged();

    void setChanged(bool changed);
    void restorePrevious();
private:
    Neuron* m_sourceNeuron;
    Neuron* m_targetNeuron;
    double m_weight;
    double m_previousWeight;
    bool m_changed;
};

#endif // CONNECTION_H
