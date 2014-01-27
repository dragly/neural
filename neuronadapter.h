#ifndef NEURONQML_H
#define NEURONQML_H

#include <QQuickItem>
#include <neuron.h>

class NeuronAdapter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QQmlListProperty<NeuronAdapter> outputNeuronAdapters READ outputNeuronAdapters)
    Q_PROPERTY(QQuickItem* neuronQML READ neuronQML WRITE setNeuronQML)
public:
    explicit NeuronAdapter(QQuickItem *parent = 0);

    Neuron *neuron() const;
    void setNeuron(Neuron *neuron);

    QString name() const
    {
        return m_name;
    }

    QQmlListProperty<NeuronAdapter> outputNeuronAdapters()
    {
        return QQmlListProperty<NeuronAdapter>(this, m_outputNeuronAdapters);
    }

    void addOutputNeuronAdapter(NeuronAdapter* neuronAdapter);

    QQuickItem* neuronQML() const
    {
        return m_neuronQML;
    }

signals:

    void nameChanged(QString arg);

public slots:

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void setNeuronQML(QQuickItem* arg)
    {
        m_neuronQML = arg;
    }

private:
    Neuron *m_neuron;
    QString m_name;
    QList<NeuronAdapter*> m_outputNeuronAdapters;
    QQuickItem* m_neuronQML;
};

#endif // NEURONQML_H
