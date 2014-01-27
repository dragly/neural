#ifndef NEURALNETWORKQML_H
#define NEURALNETWORKQML_H

#include "neuronadapter.h"

#include <QQuickItem>
#include <neuralnetwork.h>

class NeuralNetworkAdapter : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<NeuronAdapter> neuronAdapters READ neuronAdapters)
public:
    explicit NeuralNetworkAdapter(QQuickItem *parent = 0);

    QQmlListProperty<NeuronAdapter> neuronAdapters()
    {
        return QQmlListProperty<NeuronAdapter>(this, m_neuronAdapters);
    }



public slots:

    void componentComplete();
private:
    NeuralNetwork m_neuralNetwork;
    QList<NeuronAdapter*> m_neuronAdapters;
};

#endif // NEURALNETWORKQML_H
