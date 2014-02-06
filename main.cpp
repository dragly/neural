#include "qtquick2controlsapplicationviewer.h"

#include <neuralnetwork.h>
#include <neuralnetworkadapter.h>
#include <neuronadapter.h>

#include <armadillo>

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
//    qmlRegisterType<NeuralNetworkAdapter>("NeuralNetwork", 1, 0, "NeuralNetworkAdapter");
//    qmlRegisterType<NeuronAdapter>("NeuralNetwork", 1, 0, "NeuronAdapter");

//    Application app(argc, argv);

//    QtQuick2ControlsApplicationViewer viewer;
//    viewer.setMainQmlFile(QStringLiteral("qml/neural/main.qml"));
//    viewer.show();

//    return app.exec();
    NeuralNetwork network;
    network.setup(20, 1, 1, 7);
    vec inputValues = 12 * ones(1);
    network.setInputValues(inputValues);
    double previousOutput = 1;
    double currentOutput = 1;
    for(int i = 0; i < 10000; i++) {
        network.advance();
        previousOutput = currentOutput;
        currentOutput = network.outputValues()(0);
//        cout << currentOutput << endl;
    }
    cout << currentOutput << endl;
    return 0;
}
