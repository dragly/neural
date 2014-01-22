# Add more folders to ship with the application, here
folder_01.source = qml/neural
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    neuron.cpp \
    neuralnetwork.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    neuron.h \
    neuralnetwork.h

QMAKE_CXXFLAGS += -std=c++0x

OTHER_FILES += \
    qml/neural/BezierCurve.qml
