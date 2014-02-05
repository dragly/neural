#include "neuralnetwork.h"

#include <connection.h>
#include <neuron.h>

using namespace arma;

NeuralNetwork::NeuralNetwork()
{
    srand(time(NULL));
//    srand(-1);
}

void NeuralNetwork::setup(uint nNeurons, uint nInputNeurons, uint nOutputNeurons, uint nConnections) {
    if(nNeurons < nInputNeurons + nOutputNeurons) {
        cerr << "NeuralNetwork::setup: The number of neurons must be larger than the number of inputs and outputs.";
        return;
    }
    m_neurons.clear();
    m_inputNeurons.clear();
    m_outputNeurons.clear();

    for(uint i = 0; i < nNeurons; i++) {
        Neuron* neuron = new Neuron();
        m_neurons.push_back(neuron);
    }
    for(uint i = 0; i < nInputNeurons; i++) {
        Neuron* neuron = m_neurons.at(i);
        neuron->setInput(true);
        m_inputNeurons.push_back(neuron);
    }
    for(uint i = nNeurons - nOutputNeurons; i < nNeurons; i++) {
        Neuron* neuron = m_neurons.at(i);
        neuron->setOutput(true);
        m_outputNeurons.push_back(neuron);
    }

    // Set up connections
    for(Neuron* neuron : m_neurons) {
        for(uint i = 0; i < nConnections; i++) {
            uint randomIndex = randu() * nNeurons;
            Neuron* otherNeuron = m_neurons[randomIndex];
            Connection* connection = new Connection(neuron, otherNeuron);
            connection->setWeight(1.0);
            connection->setTimeLeft(randu() * 100);
            neuron->addOutputConnection(connection);
            m_connections.push_back(connection);
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

void NeuralNetwork::stepForward()
{
    for(Neuron* neuron : m_neurons) {
        neuron->sendOutput();
    }
    for(Neuron* neuron : m_neurons) {
        neuron->advance();
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
