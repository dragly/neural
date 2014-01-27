#include "qtquick2controlsapplicationviewer.h"

#include <neuralnetworkadapter.h>
#include <neuronadapter.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<NeuralNetworkAdapter>("NeuralNetwork", 1, 0, "NeuralNetworkAdapter");
    qmlRegisterType<NeuronAdapter>("NeuralNetwork", 1, 0, "NeuronAdapter");

    Application app(argc, argv);

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/neural/main.qml"));
    viewer.show();

    return app.exec();
}
