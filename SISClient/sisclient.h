#ifndef SISCLIENT_H
#define SISCLIENT_H

#include <QtNetwork>

#include "sislibrary.h"

class SisClient : public SisCommandBuilder
{
    Q_OBJECT

public:
    SisClient(const QString& address, quint16 port, CommandSink*);

    void connectToHost();
    void send(const QBuffer& ds);

signals:
    void connected();

private slots:
    void handleData();

private:
    QString m_host;
    quint16 m_port;
    CommandSink* m_pSink;
    QTcpSocket* m_pSocket;
    QBuffer m_buffer;
};

#endif // SISCLIENT_H
