#include "qtquick2controlsapplicationviewer.h"

#include <neuralnetwork.h>
#include <neuralnetworkadapter.h>
#include <neuronadapter.h>
#include <iomanip>

#include <armadillo>

using namespace std;
using namespace arma;

double target(double x) {
    double ri = 1/x;
    double ri2 = ri * ri;
    double ri6 = ri2 * ri2 * ri2;
    double ri12 = ri6 * ri6;
    return ri12 - ri6;
}

//double target(double x) {
//    double xOff = x - 2.0;
//    double xOff2 = x - 2.5;
//    return sin(12*x);
//}

int main(int argc, char* argv[])
{
//    qmlRegisterType<NeuralNetworkAdapter>("NeuralNetwork", 1, 0, "NeuralNetworkAdapter");
//    qmlRegisterType<NeuronAdapter>("NeuralNetwork", 1, 0, "NeuronAdapter");

//    Application app(argc, argv);

//    QtQuick2ControlsApplicationViewer viewer;
//    viewer.setMainQmlFile(QStringLiteral("qml/neural/main.qml"));
//    viewer.show();

//    return app.exec();
    cout << "Starting network" << endl;
    NeuralNetwork network;
    network.setup(32, 1, 1, 5);
    double minInput = 0.9;
    double maxInput = 3.0;
    vec xVec = linspace(minInput, maxInput, 60);
    double minOutput = 9999999999;
    double maxOutput = -9999999999;
    for(double x : xVec) {
        double output = target(x);
        minOutput = min(output, minOutput);
        maxOutput = max(output, maxOutput);
    }
    network.setInputRange(minInput - 3, maxInput + 3);
    network.setOutputRange(minOutput - 3, maxOutput + 3);
    for(double x : xVec) {
        network.addTargetInputOutput(x, target(x));
    }
    for(int i = 0; i < 8000000; i++) {
        network.advance();
        if(!(i % 10000)) {
            cout << setprecision(5) << "Iteration " << i << ", error " << network.error()
                 << " target " << target(2.0) << " output " << network.calculateRescaled(2.0)(0)
                 << " temperature " << network.temperature()
                 << " factor " << network.addFactor() << endl;
            ofstream outFile("test.data");
            for(double x : xVec) {
                outFile << x << " " << target(x) << " " << network.calculateRescaled(x)(0) << endl;
            }
        }
    }
//    outFile.close();
//    cout << network.calculate(12.0)(0) << endl;
//    cout << network.calculate(25.0)(0) << endl;
//    cout << "First output: " << network.outputValues()(0) << endl;
//    network.setInputValues(24 * ones(1));
//    network.setTargetOutputValues(456 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Second output: " << network.outputValues()(0) << endl;
//    network.setInputValues(12 * ones(1));
//    network.setTargetOutputValues(205 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Third output: " << network.outputValues()(0) << endl;
//    network.setInputValues(24 * ones(1));
//    network.setTargetOutputValues(456 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Fourth output: " << network.outputValues()(0) << endl;
//    network.setInputValues(12 * ones(1));
//    network.setTargetOutputValues(205 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Fifth output: " << network.outputValues()(0) << endl;
//    network.setInputValues(24 * ones(1));
//    network.setTargetOutputValues(456 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Sixth output: " << network.outputValues()(0) << endl;
//    network.setInputValues(12 * ones(1));
//    network.setTargetOutputValues(205 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Seventh output: " << network.outputValues()(0) << endl;
//    network.setInputValues(24 * ones(1));
//    network.setTargetOutputValues(456 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Eight output: " << network.outputValues()(0) << endl;
//    network.setInputValues(12 * ones(1));
//    network.setTargetOutputValues(205 * ones(1));
//    network.resetTemperature();
//    for(int i = 0; i < 10000; i++) {
//        network.advance();
////        cout << "i: " << i << " Output: " << network.outputValues()(0) << endl;
//    }
//    cout << "Ninth output: " << network.outputValues()(0) << endl;
//    network.setInputValues(24 * ones(1));
//    network.setTargetOutputValues(456 * ones(1));
//    network.calculate();
//    cout << "Final output: " << network.outputValues()(0) << endl;
    return 0;
}
