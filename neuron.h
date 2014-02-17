#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Connection;
class Neuron
{
public:
    Neuron();
    void addInputValue(double value);
    void advance();

    void sendOutput();
    void addOutputConnection(Connection* other);
    void setInputValue(double value);
    double outputValue();
    const std::vector<Connection *> &outputConnections() const;

    bool isOutput() const;
    void setOutput(bool isOutput);

    bool isInput() const;
    void setInput(bool isInput);

    void reset();
    int id();
    void setID(int id);
    void setAddition(double addition);
    bool isChanged() const;
    void setChanged(bool isChanged);
    void restorePrevious();

private:
    bool m_isInput;
    bool m_isOutput;
    double m_inputValue;
    double m_outputValue;
    int m_nInputValues;
    double m_inputValueSum;
    std::vector<Connection *> m_outputConnections;
    std::vector<double> m_outputLifeTime;
    int m_id;
    double m_addition;
    double m_previousAddition;
    bool m_changed;
};

#endif // NEURON_H
