#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron
{
public:
    Neuron();
    void addInputValue(double value);
    void advance();

    void sendOutput();
    void addOutputNeuron(Neuron* other);
    void setInputValue(double value);
    double outputValue();
    const std::vector<Neuron *>& outputNeurons() const;

private:
    bool m_isInput;
    bool m_isOutput;
    double m_inputValue;
    double m_outputValue;
    std::vector<Neuron*> m_outputNeurons;
};

#endif // NEURON_H
