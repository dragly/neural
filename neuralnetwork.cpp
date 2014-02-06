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
    m_targetOutputValues = ones(1);
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
            connection->setWeight(randu() * 1000);
            connection->setLifeTime(randu() * 100);
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

void NeuralNetwork::advance() {
    m_previousDiff = m_currentDiff;
    for(int i = 0; i < 100; i++) {
        stepForward();
    }
    m_currentDiff = m_targetOutputValues - outputValues();

    bool previousIsBetter = false;
    if(m_previousDiff.size() > 0) {
        previousIsBetter = (max(abs(m_currentDiff)) > max(abs(m_previousDiff)));
    }
    for(Connection* connection : m_connections) {
        if(connection->isChanged()) {
            connection->setPreviousBetter(previousIsBetter);
        }
    }
    transform();
}

bool compareConnections(Connection* L, Connection* R) {
    return L->lifeTime() < R->lifeTime();
}

void NeuralNetwork::transform() {
    // Reset the changed-status of all connections
    for(Connection* connection : m_connections) {
        connection->setChanged(false);
    }

    // Algorithm that randomly selects a connection based on which have lived the longest
    int lifeTimeSum = 0;
    for(Connection* connection : m_connections) {
        lifeTimeSum += connection->lifeTime();
    }

    int randomLifeTime = randu() * lifeTimeSum;
    for(Connection* connection : m_connections) {
        if(randomLifeTime < connection->lifeTime()) {
            // If the previous was better, change it by 50/50 chance
            if(connection->isPreviousBetter() && randu() > 0.5) {
                connection->restorePrevious();
            } else {
                connection->setWeight(randu() * 1000);
                connection->setChanged(true);
                connection->setLifeTime(0);
            }
            break;
        } else {
            randomLifeTime -= connection->lifeTime();
        }
    }

    // Increment all connections' lifeTime
    for(Connection* connection : m_connections) {
        connection->incrementLifeTime();
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
