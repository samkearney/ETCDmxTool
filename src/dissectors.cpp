// Copyright (c) 2017 Electronic Theatre Controls, Inc., http://www.etcconnect.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "dissectors.h"
#include <QPluginLoader>
#include <QDir>
#include "logmodel.h"

dissectors::dissectors(QObject *parent) : QObject(parent)
{
    LogModel::log(tr("Loading Dissectors"), CDL_SEV_INF, 1);
    QDir dissectorDir(QDir::currentPath());
    for (auto dissectorDll : dissectorDir.entryList(QStringList(disectorPatternMatch)))
    {
        QPluginLoader pluginLoader(dissectorDir.absoluteFilePath(dissectorDll));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            DissectorPlugin *dissector = qobject_cast<DissectorPlugin *>(plugin);
            if (dissector)
            {
                QString startCodes;
                foreach(auto sc, dissector->getStartCodes())
                    startCodes.append(QString("%1 ").arg(sc, 2, 16, QChar('0')).toUpper());
                LogModel::log(QString("Loaded Dissector %1, Accepts startcode(s) %2")
                              .arg(dissector->getProtocolName().toString())
                              .arg(startCodes),
                              CDL_SEV_INF, 1);
                m_dissectors.append(new s_DissectorList(dissector));
                m_dissectors.last()->setEnabled(dissector->enableByDefault());
            } else {
                LogModel::log(QString("Invalid Dissector %1")
                              .arg(dissectorDll)
                              , CDL_SEV_ERR
                              , 1);
            }
        }
    }
}

DissectorPlugin *dissectors::getDissector(const Packet &p)
{
    if (p.count())
    {
        auto startCode = p.at(0);
        for (auto d : m_dissectors)
        {
            if (!d->isEnabled())
                continue;
            if (d->dissector->getStartCodes().contains(startCode))
            {
                return d->dissector;
            }
        }
    }

    return Q_NULLPTR;
}
