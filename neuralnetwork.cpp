#include "neuralnetwork.h"

using namespace arma;

NeuralNetwork::NeuralNetwork()
{
}

void NeuralNetwork::setup(uint nNeurons, uint nInputNeurons, uint nOutputNeurons, uint nConnections) {
    if(nNeurons < nInputNeurons + nOutputNeurons) {
        cerr << "NeuralNetwork::setup: The number of neurons must be larger than the number of inputs and outputs.";
        return;
    }
    m_neurons.clear();
    m_inputNeurons.clear();
    m_outputNeurons.clear();

    m_neurons.resize(nNeurons);
    for(uint i = 0; i < nInputNeurons; i++) {
        Neuron &neuron = m_neurons.at(i);
        m_inputNeurons.push_back(&neuron);
    }
    for(uint i = nNeurons - nOutputNeurons; i < nNeurons; i++) {
        Neuron &neuron = m_neurons.at(i);
        m_outputNeurons.push_back(&neuron);
    }

    // Set up connections
    for(Neuron &neuron : m_neurons) {
        for(uint i = 0; i < nConnections; i++) {
            uint randomIndex = rand() / RAND_MAX * nNeurons;
            Neuron* otherNeuron = &(m_neurons.at(randomIndex));
            neuron.addOutputNeuron(otherNeuron);
        }
    }
}

void NeuralNetwork::setInputValues(vec inputValues)
{
    if(inputValues.size() != m_inputNeurons.size()) {
        cerr << "NeuralNetwork::setInputs: Number of input values does not match the number of input neurons in network." << endl;
        cerr << "NeuralNetwork::setInputs: Cannot set values of input neurons." << endl;
        return;
    }
    for(uint i = 0; i < inputValues.size(); i++) {
        Neuron *inputNeuron = m_inputNeurons.at(i);
        inputNeuron->setInputValue(inputValues.at(i));
    }
}

void NeuralNetwork::advance()
{
    for(Neuron &neuron : m_neurons) {
        neuron.sendOutput();
    }
    for(Neuron &neuron : m_neurons) {
        neuron.advance();
    }
}

vec NeuralNetwork::outputValues() {
    uint nValues = m_outputNeurons.size();
    vec output = zeros(nValues);
    for(uint i = 0; i < nValues; i++) {
        Neuron *outputNeuron = m_outputNeurons.at(i);
        output(i) = outputNeuron->outputValue();
    }
    return output;
}
