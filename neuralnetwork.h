#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <armadillo>

class Connection;
class Neuron;
class NeuralNetwork
{
public:
    NeuralNetwork();

    void stepForward();
    void setInputValues(arma::vec inputValues);
    void setup(uint nNeurons, uint nInputNeurons, uint nOutputNeurons, uint nConnections);
    arma::vec outputValues();

    const std::vector<Neuron*>& neurons();
    void transform();
    void advance();
private:
    std::vector<Neuron*> m_neurons;
    std::vector<Neuron*> m_inputNeurons;
    std::vector<Neuron*> m_outputNeurons;
    std::vector<Connection*> m_connections;
    arma::vec m_targetOutputValues;
    arma::vec m_previousDiff;
    arma::vec m_currentDiff;
};

inline const std::vector<Neuron *> &NeuralNetwork::neurons()
{
    return m_neurons;
}

#endif // NEURALNETWORK_H
