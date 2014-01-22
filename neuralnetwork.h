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
private:
    std::vector<Neuron> m_neurons;
    std::vector<Neuron*> m_inputNeurons;
    std::vector<Neuron*> m_outputNeurons;
};

#endif // NEURALNETWORK_H
