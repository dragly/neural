#ifndef CONNECTION_H
#define CONNECTION_H

class Neuron;
class Connection
{
public:
    Connection(Neuron* source, Neuron* target);

    int lifeTime() const;
    void setLifeTime(int lifeTime);

    double weight() const;
    void setWeight(double weight);

    Neuron* sourceNeuron();
    Neuron* targetNeuron();

    bool isChanged();

    void incrementLifeTime();
    void setChanged(bool changed);
    void restorePrevious();
private:
    Neuron* m_sourceNeuron;
    Neuron* m_targetNeuron;
    int m_lifeTime;
    double m_weight;
    double m_previousWeight;
    bool m_changed;
};

#endif // CONNECTION_H
