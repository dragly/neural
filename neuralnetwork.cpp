#include "neuralnetwork.h"

#include <connection.h>
#include <neuron.h>

using namespace arma;

NeuralNetwork::NeuralNetwork() :
    m_previousDiff(INFINITY),
    m_currentDiff(1),
    m_nAdvances(1)
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

    int nHiddenLayers = 2;
    int nPerLayer = (nNeurons - nInputNeurons - nOutputNeurons) / nHiddenLayers;

    vector<Neuron*> firstLayer;
    vector<Neuron*> secondLayer;

    for(uint i = 0; i < nNeurons; i++) {
        Neuron* neuron = new Neuron();
        neuron->setID(i);
        m_neurons.push_back(neuron);
        neuron->setAddition(randn() * 1);
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
    for(uint i = nInputNeurons; i < nInputNeurons + nPerLayer; i++) {
        Neuron* neuron = m_neurons.at(i);
        firstLayer.push_back(neuron);
    }
    for(uint i = nInputNeurons + nPerLayer; i < nInputNeurons + 2 * nPerLayer; i++) {
        Neuron* neuron = m_neurons.at(i);
        secondLayer.push_back(neuron);
    }

    for(Neuron* neuron : m_inputNeurons) {
        for(Neuron* otherNeuron : firstLayer) {
            Connection* connection = new Connection(neuron, otherNeuron);
            neuron->addOutputConnection(connection);
            m_connections.push_back(connection);
        }
    }

    for(Neuron* neuron : firstLayer) {
        for(Neuron* otherNeuron : secondLayer) {
            Connection* connection = new Connection(neuron, otherNeuron);
            neuron->addOutputConnection(connection);
            m_connections.push_back(connection);
        }
    }

    for(Neuron* neuron : secondLayer) {
        for(Neuron* otherNeuron : m_outputNeurons) {
            Connection* connection = new Connection(neuron, otherNeuron);
            neuron->addOutputConnection(connection);
            m_connections.push_back(connection);
        }
    }

    // Set up connections
//    int firstIndex = 0;
//    for(Neuron* neuron : m_neurons) {
//        if(neuron->isOutput()) {
//            continue;
//        }
//        for(uint i = 0; i < nConnections; i++) {
//            uint randomIndex = randu() * nNeurons;
//            Neuron* otherNeuron = m_neurons.at(randomIndex);
//            cout << firstIndex << " to " << randomIndex << "/" << nNeurons << " is output: " << otherNeuron->isOutput() << endl;
//            if(otherNeuron->isInput()) {
//                i--;
//                continue;
//            }
//            if(otherNeuron == neuron) {
//                i--;
//                continue;
//            }
//            bool existsAlready = false;

//            for(Connection* existingConnection : m_connections) {
//                if(existingConnection->sourceNeuron() == neuron &&
//                        existingConnection->targetNeuron() == otherNeuron) {
//                    existsAlready = true;
//                }
//            }
//            if(existsAlready) {
//                i--;
//                continue;
//            }
//            Connection* connection = new Connection(neuron, otherNeuron);
//            //            connection->setWeight(randu() * 1000);
//            connection->setWeight(randn() * 1);
//            neuron->addOutputConnection(connection);
//            m_connections.push_back(connection);
//            cout << "Done!" << endl;
//        }
//        firstIndex++;
//    }

    pair<vec, vec> defaultInputRanges;
    defaultInputRanges.first = zeros(nInputNeurons);
    defaultInputRanges.second = zeros(nInputNeurons);
    setInputRanges(defaultInputRanges);
    pair<vec, vec> defaultOutputRanges;
    defaultOutputRanges.first = zeros(nOutputNeurons);
    defaultOutputRanges.second = zeros(nOutputNeurons);
    setOutputRanges(defaultOutputRanges);
    resetTemperature();
}

vec NeuralNetwork::normalizeInput(double value) {
    vec values = ones(1) * value;
    return normalizeInput(values);
}

vec NeuralNetwork::normalizeOutput(double value) {
    vec values = ones(1) * value;
    return normalizeOutput(values);
}

vec NeuralNetwork::deNormalizeInput(double value) {
    vec values = ones(1) * value;
    return deNormalizeInput(values);
}

vec NeuralNetwork::deNormalizeOutput(double value) {
    vec values = ones(1) * value;
    return deNormalizeOutput(values);
}

vec NeuralNetwork::normalizeInput(vec values) {
    return (values - m_inputRanges.first) / (m_inputRanges.second - m_inputRanges.first) * 2 - 1;
}

vec NeuralNetwork::deNormalizeInput(vec values) {
    return ((values + 1) / 2) % (m_inputRanges.second - m_inputRanges.first) + m_inputRanges.first;
}

vec NeuralNetwork::normalizeOutput(vec values) {
    return (values - m_outputRanges.first) / (m_outputRanges.second - m_outputRanges.first);
}

vec NeuralNetwork::deNormalizeOutput(vec values) {
    return values % (m_outputRanges.second - m_outputRanges.first) + m_outputRanges.first;
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

void NeuralNetwork::addTargetInputOutput(double input, double output)
{
    addTargetInputOutput(ones(1) * input, ones(1) * output);
}

void NeuralNetwork::addTargetInputOutput(vec input, vec output)
{
    if(input.size() != m_inputNeurons.size()) {
        cerr << "NeuralNetwork::addTargetInputOutput: Number of input values does not match the number of input neurons in network." << endl;
        return;
    }
    if(output.size() != m_outputNeurons.size()) {
        cerr << "NeuralNetwork::addTargetInputOutput: Number of output values does not match the number of output neurons in network." << endl;
        return;
    }
//    cout << "Target input/output received as " << input << output << endl;
    vec normalizedInput = normalizeInput(input);
    vec normalizedOutput = normalizeOutput(output);
//    cout << "Target input/output set to " << normalizedInput << " " << normalizedOutput << endl;
    m_targetInputOutput.push_back(std::pair<vec, vec>(normalizedInput, normalizedOutput));
}

void NeuralNetwork::resetTemperature()
{
    m_temperature = 1000000;
    m_nAdvances = 1;
}


void NeuralNetwork::advance() {
    transform();
    m_currentDiff = 1;
    for(std::pair<vec, vec> targetInputOutput : m_targetInputOutput) {
        vec targetInput = targetInputOutput.first;
        vec targetOutput = targetInputOutput.second;
        vec outputValues = calculate(targetInput);
        double thisDiff = max(abs(targetOutput - outputValues));
        m_currentDiff += thisDiff*thisDiff;
    }
    double deltaDiff = m_currentDiff - m_previousDiff;
//    double deltaDiff = randn();
//    cout << deltaDiff << endl;
//    if(m_currentDiff < m_previousDiff) {
    if(randu() < exp(-deltaDiff / (m_temperature / m_nAdvances))) {
//        cout << "Keep" << endl;
        m_previousDiff = m_currentDiff;
    } else {
//        cout << "Restore" << endl;
        restore();
    }
    if(m_nAdvances < 1e5) {
        m_temperature *= 0.999;
    }
    m_nAdvances++;
}

vec NeuralNetwork::calculate(double inputValue) {
    return calculate(ones(1) * inputValue);
}

vec NeuralNetwork::calculate(vec inputValues) {
    for(Neuron* neuron : m_neurons) {
        neuron->reset();
    }
    for(uint i = 0; i < inputValues.size(); i++) {
        Neuron *inputNeuron = m_inputNeurons.at(i);
        inputNeuron->setInputValue(inputValues.at(i));
    }
    for(uint i = 0; i < 4; i++) {
        stepForward();
    }
    uint nValues = m_outputNeurons.size();
    vec output = zeros(nValues);
    for(uint i = 0; i < nValues; i++) {
        Neuron *outputNeuron = m_outputNeurons.at(i);
        output(i) = outputNeuron->outputValue();
    }
    return output;
}

vec NeuralNetwork::calculateRescaled(double inputValue) {
    return calculateRescaled(inputValue * ones(1));
}

vec NeuralNetwork::calculateRescaled(vec inputValues) {
    vec normalizedInputValues = normalizeInput(inputValues);
    vec normalizedOutputValues = calculate(normalizedInputValues);
    vec scaledOutputValues = deNormalizeOutput(normalizedOutputValues);
    return scaledOutputValues;
}

void NeuralNetwork::setInputRange(double start, double stop)
{
    pair<vec, vec> ranges;
    ranges.first = ones(1) * start;
    ranges.second = ones(1) * stop;
    setInputRanges(ranges);
}

void NeuralNetwork::setOutputRange(double start, double stop)
{
    pair<vec, vec> ranges;
    ranges.first = ones(1) * start;
    ranges.second = ones(1) * stop;
    setOutputRanges(ranges);
}
pair<vec, vec> NeuralNetwork::inputRanges() const
{
    return m_inputRanges;
}

void NeuralNetwork::setInputRanges(const pair<vec, vec> &inputRanges)
{
    m_inputRanges = inputRanges;
}
pair<vec, vec> NeuralNetwork::outputRanges() const
{
    return m_outputRanges;
}

void NeuralNetwork::setOutputRanges(const pair<vec, vec> &outputRanges)
{
    m_outputRanges = outputRanges;
}

void NeuralNetwork::restore() {
    // Reset the changed-status of all connections
    for(Connection* connection : m_connections) {
        if(connection->isChanged()) {
            connection->restorePrevious();
            connection->setChanged(false);
        }
    }
    // Reset the changed-status of all neurons
    for(Neuron* neuron : m_neurons) {
        if(neuron->isChanged()) {
            neuron->restorePrevious();
            neuron->setChanged(false);
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
    double weightFactor = 100;
    connection->setWeight(randn() * weightFactor);
    connection->setChanged(true);


    // Reset the changed-status of all neurons
    for(Neuron* neuron : m_neurons) {
        neuron->setChanged(false);
    }
    // Select random neuron
    uint neuronID = randu() * m_neurons.size();
    Neuron* neuron = m_neurons.at(neuronID);
    double addFactor = 100;
    neuron->setAddition(randn() * addFactor);
    neuron->setChanged(true);
}
