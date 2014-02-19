#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <armadillo>

using namespace arma;
using namespace std;

class Connection;
class Neuron;
class NeuralNetwork
{
public:
    NeuralNetwork();

    void stepForward();
    void addTargetInputOutput(double input, double output);
    void addTargetInputOutput(vec input, vec output);
//    void setInputValues(vec inputValues);
    void setup(uint nNeurons, uint nInputNeurons, uint nOutputNeurons, uint nConnections);
    vec outputValues();

    const vector<Neuron*>& neurons();
    void transform();
    void advance();
    void restore();
    void reset();
    vec targetOutputValues() const;
//    void setTargetOutputValues(const vec &targetOutputValues);
    void resetTemperature();

    vec calculate(double inputValue);
    vec calculate(vec inputValues);

    void setInputRange(double start, double stop);
    void setOutputRange(double start, double stop);

    pair<vec, vec> inputRanges() const;
    void setInputRanges(const pair<vec, vec> &inputRanges);

    pair<vec, vec> outputRanges() const;
    void setOutputRanges(const pair<vec, vec> &outputRanges);

    vec calculateRescaled(double inputValue);
    vec calculateRescaled(vec inputValues);
    vec deNormalizeOutput(vec values);
    vec normalizeOutput(vec values);
    vec deNormalizeInput(vec values);
    vec normalizeInput(vec values);
    vec deNormalizeOutput(double value);
    vec deNormalizeInput(double value);
    vec normalizeOutput(double value);
    vec normalizeInput(double value);
    double temperature();
    double error();
    double addFactor() const;

private:
    vector<Neuron*> m_neurons;
    vector<Neuron*> m_inputNeurons;
    vector<Neuron*> m_outputNeurons;
    vector<Connection*> m_connections;
    vector<pair<vec, vec>> m_targetInputOutput;
    pair<vec, vec> m_inputRanges;
    pair<vec, vec> m_outputRanges;
    vec m_targetOutputValues;
    double m_previousDiff;
    double m_currentDiff;
    double m_temperature;
    double m_previousError;
    double m_currentError;
    int m_nAdvances;
    uint m_nStepsPerCalculation;
    double m_addFactor;
};

inline const vector<Neuron *> &NeuralNetwork::neurons()
{
    return m_neurons;
}

inline double NeuralNetwork::temperature() {
    return m_temperature;
}

inline double NeuralNetwork::error() {
    return m_previousError;
}

#endif // NEURALNETWORK_H
