#include "qtquick2controlsapplicationviewer.h"

#include <neuralnetwork.h>
#include <neuralnetworkadapter.h>
#include <neuronadapter.h>
#include <iomanip>

#include <armadillo>

using namespace std;
using namespace arma;

double twoParticlePotential(double r) {
    double HSiO = 11.387;
    double etaSiO = 9;
    double ZSi = 1.6;
    double aSi = 0.00;
    double ZO = -0.8;
    double aO = 2.4;
    double r4s = 4.43;
    return HSiO / pow(r, etaSiO) + ZSi * ZO / r - 0.5 * (aSi * ZO * ZO + aO * ZSi * ZSi) / pow(r,4) * exp(-r/r4s);
}

double potential(double rij, double rik, double angle) {
    // Potential for Si-O-Si bond
    double Bijk = 1.4;
    double r0 = 2.6;
    double theta0 = 141.0 / 360. * M_PI;
    double V2 = twoParticlePotential(rij);
    V2 += twoParticlePotential(rik);
    double f = 0;
    if(rij < r0 && rik < r0) {
        f = exp(1 / (rij - r0) + 1 / (rik - r0));
    }
    double sqrtp = (cos(angle) - cos(theta0));
    double p = sqrtp * sqrtp;
    double V3 = Bijk * f * p;
    return V2 + V3;
}

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
    //        qmlRegisterType<NeuralNetworkAdapter>("NeuralNetwork", 1, 0, "NeuralNetworkAdapter");
    //        qmlRegisterType<NeuronAdapter>("NeuralNetwork", 1, 0, "NeuronAdapter");

    //        Application app(argc, argv);

    //        QtQuick2ControlsApplicationViewer viewer;
    //        viewer.setMainQmlFile(QStringLiteral("qml/neural/main.qml"));
    //        viewer.show();

    //        return app.exec();
    //    cout << "Starting network" << endl;
    NeuralNetwork network;
    network.setup(30, 3, 1, 7);
    double minInput = 1.2;
    double maxInput = 2.7;
    double minOutput = 9999999999;
    double maxOutput = -9999999999;
    int nPracticeValues = 10;
    int nTestValues = 100;
    for(int i = 0; i < nPracticeValues; i++) {
        for(int j = 0; j < nPracticeValues; j++) {
            //            double rij = minInput + randu() * (maxInput - minInput);
            double rij = minInput + double(i) / nPracticeValues * (maxInput - minInput);
            //        double rik = minInput + randu() * (maxInput - minInput);
            double rik = rij;
            //            double angle = M_PI * randu();
            double angle = M_PI * double(j) / nPracticeValues;
            double output = potential(rij, rik, angle);
            minOutput = min(minOutput, output);
            maxOutput = max(maxOutput, output);
        }
    }
    vec minInputRanges;
    minInputRanges << 0.0 << 0.0 << 0.0;
    vec maxInputRanges;
    maxInputRanges << maxInput + 1.0 << maxInput + 1.0 << M_PI;
    network.setInputRanges(pair<vec,vec>(minInputRanges, maxInputRanges));
    network.setOutputRange(minOutput - 3, maxOutput + 3);
    for(int i = 0; i < nPracticeValues; i++) {
        for(int j = 0; j < nPracticeValues; j++) {
            //            double rij = minInput + randu() * (maxInput - minInput);
            double rij = minInput + double(i) / nPracticeValues * (maxInput - minInput);
            //        double rik = minInput + randu() * (maxInput - minInput);
            double rik = rij;
            //            double angle = M_PI * randu();
            double angle = M_PI * double(j) / nPracticeValues;
            vec input;
            input << rij << rik << angle;
            vec output;
            output << potential(rij, rik, angle);
            network.addTargetInputOutput(input, output);
        }
    }
    for(int i = 0; i < 80000000; i++) {
        network.advance();
        if(!(i % 1000)) {
            cout << setprecision(5) << "Iteration " << i << ", error " << network.error()
                 << " temperature " << network.temperature()
                 << " accept ratio " << network.acceptCount() / double(network.totalCount())
                 << " true accept ratio " << network.trueAcceptCount() / double(network.totalCount())
                 << " ratio ratio " << network.trueAcceptCount() / double(network.acceptCount())
                 << " factor " << network.addFactor() << endl;
            ofstream outFile("plot.data");
            ofstream outFileTarget("plot_target.data");
            for(int i = 0; i < nTestValues; i++) {
                for(int j = 0; j < nTestValues; j++) {
                    double rij = minInput + double(i) / nTestValues * (maxInput - minInput);
                    double rik = rij;
                    //                    double rik = minInput;
                    double angle = M_PI * double(j) / nTestValues;
                    vec input;
                    input << rij << rik << angle;
                    double output = network.calculateRescaled(input)(0);
                    double targetOutput = potential(rij, rik, angle);
                    outFile << output << " ";
                    outFileTarget << targetOutput << " ";
                }
                outFile << endl;
                outFileTarget << endl;
            }
            outFile.close();
            outFileTarget.close();
            network.resetCounters();
        }
        //        if(!(i % 30000)) {
        //            double rij = minInput + randu() * (maxInput - minInput);
        //            //        double rik = distRangeLow + randu() * (distRangeHigh - distRangeLow);
        //            double rik = minInput + randu() * (maxInput - minInput);
        //            double angle = M_PI * randu();
        //            vec input;
        //            input << rij << rik << angle;
        //            vec output;
        //            output << potential(rij, rik, angle);
        //            network.addTargetInputOutput(input, output);
        //            network.resetTemperature();
        //        }
    }
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
    //    return 0;
}
