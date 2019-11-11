#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <QObject>
#include <QJSEngine>
#include <QThread>
#include "capturedevice.h"

class ScriptDMX : public QObject
{
    Q_OBJECT
public:
    ScriptDMX(ICaptureDevice *device);
public slots:
    void setLevel(quint16 address, quint8 value);
    void sleep(int seconds);
    void enable();
    void disable();
private:
    quint8 m_dmxLevels[512];
    ICaptureDevice *m_captureDevice;
};

class Scripting : public QObject
{
    Q_OBJECT
public:
    explicit Scripting(ICaptureDevice *device);
    QString lastErrorDescription() { return m_lastError;}
    int lastErrorLine() { return m_lastErrorLine;}
signals:
    void finished(bool error);
public slots:
    bool isRunning();
    void run(const QString &script);
private:
    QJSEngine m_engine;
    bool m_running;
    ICaptureDevice *m_captureDevice;
    ScriptDMX *m_dmx;
    QString m_lastError;
    int m_lastErrorLine;
};

#endif // SCRIPTING_H
