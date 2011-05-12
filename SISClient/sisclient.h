#ifndef SISCLIENT_H
#define SISCLIENT_H

#include <QtNetwork>

#include "sislibrary.h"

class ICommandSink;

class SisClient : public SisCommandBuilder
{
    Q_OBJECT

public:
    SisClient(const QString& address, quint16 port, ICommandSink*);

    void connectToHost();
    void send(const QBuffer& ds);

private slots:
    void handleData();

private:
    QString m_host;
    quint16 m_port;
    ICommandSink* m_pSink;
    QTcpSocket* m_pSocket;
};

#endif // SISCLIENT_H
