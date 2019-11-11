#include "scripting.h"
#include <QDebug>

ScriptDMX::ScriptDMX(ICaptureDevice *device) : QObject(Q_NULLPTR), m_captureDevice(device)
{
    memset(m_dmxLevels, 0, 512);
}

void ScriptDMX::setLevel(quint16 address, quint8 value)
{
    qDebug() << "Setting DMX" << address << "to" << value;
    m_dmxLevels[address] = value;
    if(m_captureDevice)
        m_captureDevice->setDmxLevels(m_dmxLevels, 512);
}

void ScriptDMX::sleep(int seconds)
{
    QThread::msleep(seconds);
}


void ScriptDMX::enable()
{
    if(m_captureDevice)
        m_captureDevice->setDmxEnabled(true);
}

void ScriptDMX::disable()
{
    if(m_captureDevice)
        m_captureDevice->setDmxEnabled(false);
}

Scripting::Scripting(ICaptureDevice *device) : QObject(Q_NULLPTR)
{
    QThread *thread = new QThread();
    m_running = false;
    m_captureDevice = device;
    this->moveToThread(thread);
    thread->start();
    m_dmx = new ScriptDMX(device);
    m_dmx->setObjectName("dmx");

    m_engine.installExtensions(QJSEngine::ConsoleExtension);
}

bool Scripting::isRunning()
{
    return m_running;
}

void Scripting::run(const QString &script)
{
    if(QThread::currentThread()!=this->thread())
    {
        QMetaObject::invokeMethod(this, "run", Qt::QueuedConnection, Q_ARG(QString, script));
        return;
    }

    QJSValue dmxValue = m_engine.newQObject(m_dmx);
    m_engine.globalObject().setProperty("dmx", dmxValue);

    m_running = true;
    QJSValue value = m_engine.evaluate(script);
    m_running = false;

    emit finished(value.isError());

    if(value.isError())
    {
        m_lastError = value.property("message").toString();
        m_lastErrorLine = value.property("lineNumber").toInt();
    }
}
