#include "qtquick2controlsapplicationviewer.h"

#include <neuralnetwork.h>
#include <neuralnetworkadapter.h>
#include <neuronadapter.h>

#include <armadillo>

using namespace std;
using namespace arma;

int main()
{
//    qmlRegisterType<NeuralNetworkAdapter>("NeuralNetwork", 1, 0, "NeuralNetworkAdapter");
//    qmlRegisterType<NeuronAdapter>("NeuralNetwork", 1, 0, "NeuronAdapter");

//    Application app(argc, argv);

//    QtQuick2ControlsApplicationViewer viewer;
//    viewer.setMainQmlFile(QStringLiteral("qml/neural/main.qml"));
//    viewer.show();

//    return app.exec();
    NeuralNetwork network;
    network.setup(20, 1, 1, 4);
    network.setInputValues(12 * ones(1));
    network.setTargetOutputValues(205 * ones(1));
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "First output: " << network.outputValues()(0) << endl;
    network.setInputValues(24 * ones(1));
    network.setTargetOutputValues(456 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Second output: " << network.outputValues()(0) << endl;
    network.setInputValues(12 * ones(1));
    network.setTargetOutputValues(205 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Third output: " << network.outputValues()(0) << endl;
    network.setInputValues(24 * ones(1));
    network.setTargetOutputValues(456 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Fourth output: " << network.outputValues()(0) << endl;
    network.setInputValues(12 * ones(1));
    network.setTargetOutputValues(205 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Fifth output: " << network.outputValues()(0) << endl;
    network.setInputValues(24 * ones(1));
    network.setTargetOutputValues(456 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Sixth output: " << network.outputValues()(0) << endl;
    network.setInputValues(12 * ones(1));
    network.setTargetOutputValues(205 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Seventh output: " << network.outputValues()(0) << endl;
    network.setInputValues(24 * ones(1));
    network.setTargetOutputValues(456 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Eight output: " << network.outputValues()(0) << endl;
    network.setInputValues(12 * ones(1));
    network.setTargetOutputValues(205 * ones(1));
    network.resetTemperature();
    for(int i = 0; i < 10000; i++) {
        network.advance();
//        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
    }
    cout << "Ninth output: " << network.outputValues()(0) << endl;
    network.setInputValues(24 * ones(1));
    network.setTargetOutputValues(456 * ones(1));
    network.calculate();
    cout << "Final output: " << network.outputValues()(0) << endl;
    return 0;
}
