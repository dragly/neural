#include "neuralnetwork.h"

#include <connection.h>
#include <neuron.h>

using namespace arma;

NeuralNetwork::NeuralNetwork() :
    m_previousDiff(INFINITY),
    m_currentDiff(1),
    m_nAdvances(1),
    m_stepsSinceWeightRefresh(1),
    m_acceptCount(0),
    m_rejectCount(0),
    m_totalCount(0)
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

    bool layered = true;
    if(layered) {
        // Layered connections

        int nHiddenLayers = 3;
        int nPerLayer = (nNeurons - nInputNeurons - nOutputNeurons) / nHiddenLayers;

        vector<Neuron*> firstLayer;
        vector<Neuron*> secondLayer;
        vector<Neuron*> thirdLayer;
        for(uint i = nInputNeurons; i < nInputNeurons + nPerLayer; i++) {
            Neuron* neuron = m_neurons.at(i);
            firstLayer.push_back(neuron);
        }
        for(uint i = nInputNeurons + nPerLayer; i < nInputNeurons + 2 * nPerLayer; i++) {
            Neuron* neuron = m_neurons.at(i);
            secondLayer.push_back(neuron);
        }
        for(uint i = nInputNeurons + 2*nPerLayer; i < nInputNeurons + 3 * nPerLayer; i++) {
            Neuron* neuron = m_neurons.at(i);
            thirdLayer.push_back(neuron);
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
            for(Neuron* otherNeuron : thirdLayer) {
                Connection* connection = new Connection(neuron, otherNeuron);
                neuron->addOutputConnection(connection);
                m_connections.push_back(connection);
            }
        }

        for(Neuron* neuron : thirdLayer) {
            for(Neuron* otherNeuron : m_outputNeurons) {
                Connection* connection = new Connection(neuron, otherNeuron);
                neuron->addOutputConnection(connection);
                m_connections.push_back(connection);
            }
        }
    } else {
        // Set up connections
        int firstIndex = 0;
        for(Neuron* neuron : m_neurons) {
            if(neuron->isOutput()) {
                continue;
            }
            for(uint i = 0; i < nConnections; i++) {
                uint randomIndex = randu() * nNeurons;
                Neuron* otherNeuron = m_neurons.at(randomIndex);
                cout << firstIndex << " to " << randomIndex << "/" << nNeurons << " is output: " << otherNeuron->isOutput() << endl;
                if(otherNeuron->isInput()) {
                    i--;
                    continue;
                }
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
                neuron->addOutputConnection(connection);
                m_connections.push_back(connection);
                cout << "Done!" << endl;
            }
            firstIndex++;
        }
    }

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
double NeuralNetwork::addFactor() const
{
    return m_addFactor;
}

void NeuralNetwork::resetCounters()
{
    m_acceptCount = 0;
    m_rejectCount = 0;
    m_totalCount = 0;
    m_trueAcceptCount = 0;
}

int NeuralNetwork::acceptCount() {
    return m_acceptCount;
}

int NeuralNetwork::trueAcceptCount()
{
    return m_trueAcceptCount;
}

int NeuralNetwork::rejectCount() {
    return m_rejectCount;
}

int NeuralNetwork::totalCount() {
    return m_totalCount;
}

double NeuralNetwork::weightMax()
{
    return m_weightMaxPrevious;
}

double NeuralNetwork::additionMax()
{
    return m_additionMaxPrevious;
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
    m_temperature = 100000;
    //    m_nAdvances = 1;
    m_stepsSinceWeightRefresh = 1;
}


void NeuralNetwork::advance() {
    transform();
    m_currentDiff = 0.0;
    for(std::pair<vec, vec> targetInputOutput : m_targetInputOutput) {
        vec targetInput = targetInputOutput.first;
        vec targetOutput = targetInputOutput.second;
        vec outputValues = calculate(targetInput);
        vec diff = targetOutput - outputValues;
        double thisDiff = dot(diff, diff);
        //        m_currentDiff = thisDiff;
        m_currentDiff += thisDiff*thisDiff;
    }
    m_currentError = m_currentDiff;
    m_weightMaxCurrent = 0;
    for(Connection *connection : m_connections) {
//        m_currentDiff += 1e-9 * connection->weight() * connection->weight();
        m_weightMaxCurrent = max(m_weightMaxCurrent, connection->weight());
    }
    m_additionMaxCurrent = 0;
    for(Neuron *neuron : m_neurons) {
//        m_currentDiff += 1e-9 * neuron->addition() * neuron->addition();
        m_additionMaxCurrent = max(m_additionMaxCurrent, neuron->addition());
    }
    double deltaDiff = m_currentDiff - m_previousDiff;
    //    double deltaDiff = randn();
    //    cout << deltaDiff << endl;
    //    if(m_currentDiff < m_previousDiff) {
    double acceptanceProbability = exp(-deltaDiff / (m_temperature));
    if(randu() < acceptanceProbability) {
        //        cout << "Keep" << endl;
        m_acceptCount += 1;
        m_previousDiff = m_currentDiff;
        m_previousError = m_currentError;
        m_additionMaxPrevious = m_additionMaxCurrent;
        m_weightMaxPrevious = m_weightMaxCurrent;
        if(acceptanceProbability >= 1) {
            m_trueAcceptCount += 1;
        }
    } else {
        //        cout << "Restore" << endl;
        m_rejectCount += 1;
        restore();
    }
    m_totalCount += 1;
    m_temperature = 1e-10 + min(10.0, 1e-2*m_previousError);
//        m_temperature = 0.01;
    m_nAdvances++;
    m_stepsSinceWeightRefresh++;
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
//    double weightFactor = pow(10.0, 4*randn());
        double weightFactor = 0.001 + min(4.0, 1e9 / pow(m_nAdvances, 2.0));
    //    double weightFactor = m_previousError*10;
    connection->setWeight(randn() * weightFactor);
    connection->setChanged(true);


    // Reset the changed-status of all neurons
    for(Neuron* neuron : m_neurons) {
        neuron->setChanged(false);
    }
    // Select random neuron
    uint neuronID = randu() * m_neurons.size();
    Neuron* neuron = m_neurons.at(neuronID);
//    m_addFactor = pow(10.0, 4*randn());;
        m_addFactor = 0.001 + min(4.0, 1e9 / pow(m_nAdvances, 2.0));
    //    m_addFactor = m_previousError*10;
    neuron->setAddition(randn() * m_addFactor);
    neuron->setChanged(true);
}
