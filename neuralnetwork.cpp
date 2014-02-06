#include "neuralnetwork.h"

#include <connection.h>
#include <neuron.h>

using namespace arma;

NeuralNetwork::NeuralNetwork() :
    m_previousDiff(INFINITY),
    m_currentDiff(1),
    m_nAdvances(1)
{
    //    srand(time(NULL));
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
        neuron->setID(i);
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
            if(otherNeuron == neuron) {
                i--;
                continue;
            }
            bool existsAlready = false;

            for(Connection* existingConnection : m_connections) {
                if(existingConnection->sourceNeuron() == neuron &&
                        existingConnection->targetNeuron() == otherNeuron) {
                    existsAlready = true;
                }
            }
            if(existsAlready) {
                i--;
                continue;
            }
            Connection* connection = new Connection(neuron, otherNeuron);
            //            connection->setWeight(randu() * 1000);
            connection->setWeight(randn() * 1);
            connection->setLifeTime(randu() * 100);
            neuron->addOutputConnection(connection);
            m_connections.push_back(connection);
        }
    }
    resetTemperature();
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
        //        cout << "Neuron " << inputNeuron->id() << " has output value " << inputNeuron->outputValue() << endl;
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

void NeuralNetwork::reset() {
    for(Neuron* neuron : m_neurons) {
        neuron->reset();
    }
}
arma::vec NeuralNetwork::targetOutputValues() const
{
    return m_targetOutputValues;
}

void NeuralNetwork::setTargetOutputValues(const arma::vec &targetOutputValues)
{
    m_targetOutputValues = targetOutputValues;
}

void NeuralNetwork::resetTemperature()
{
    m_temperature = 1000;
    m_nAdvances = 1;
}


void NeuralNetwork::advance() {
    transform();
    calculate();
    m_currentDiff = max(abs(m_targetOutputValues - outputValues()));

    //    cout << m_currentDiff << "/" << m_previousDiff << endl;
    //    if(randu() <= m_currentDiff / m_previousDiff)  {
    //    cout << 0.01 * exp(-fabs(m_currentDiff)) << endl;
    double deltaDiff = m_currentDiff - m_previousDiff;
    if(randu() < exp(-deltaDiff / (m_temperature / m_nAdvances))) {
//    if(m_currentDiff < m_previousDiff || randu() > 0.999 + 0.001 * exp(-m_currentDiff)) { // || randu() > 0.999 + 0.0005 * exp(-m_previousDiff))  {
        //                cout << "Keeping" << endl;
        //        m_previousDiff = m_currentDiff;
        m_previousDiff = m_currentDiff;
    } else {
        //        cout << "Restoring" << endl;
        restore();
        calculate();
    }
    m_nAdvances++;
}

void NeuralNetwork::calculate() {
    reset();
    for(int i = 0; i < 100; i++) {
        stepForward();
    }
}

void NeuralNetwork::restore() {
    // Reset the changed-status of all connections
    for(Connection* connection : m_connections) {
        if(connection->isChanged()) {
            connection->restorePrevious();
            connection->setChanged(false);
        }
    }
}

void NeuralNetwork::transform() {
    // Reset the changed-status of all connections
    for(Connection* connection : m_connections) {
        connection->setChanged(false);
    }
    // Select random connection
    uint connectionID = randu() * m_connections.size();
    Connection* connection = m_connections.at(connectionID);
    double weightFactor = 10;
    if(m_currentDiff < 1) {
        weightFactor = 0.1;
    }
    if(m_currentDiff < 0.1) {
        weightFactor = 0.01;
    }
    connection->setWeight(randn() * weightFactor);
    connection->setChanged(true);
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
