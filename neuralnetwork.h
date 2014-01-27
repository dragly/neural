#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <neuron.h>
#include <vector>
#include <armadillo>

class NeuralNetwork
{
public:
    NeuralNetwork();

    void advance();
    void setInputValues(arma::vec inputValues);
    void setup(uint nNeurons, uint nInputNeurons, uint nOutputNeurons, uint nConnections);
    arma::vec outputValues();

    const std::vector<Neuron*>& neurons();
private:
    std::vector<Neuron*> m_neurons;
    std::vector<Neuron*> m_inputNeurons;
    std::vector<Neuron*> m_outputNeurons;
};

inline const std::vector<Neuron *> &NeuralNetwork::neurons()
{
    return m_neurons;
}

#endif // NEURALNETWORK_H
