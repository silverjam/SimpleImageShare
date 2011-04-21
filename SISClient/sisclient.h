#ifndef SISCLIENT_H
#define SISCLIENT_H

#include <QtNetwork>

class SISClient
    : QObject
{
    Q_OBJECT

public:
    SISClient(const QString& address, quint16 port);

    void connectToHost();

private slots:
    void handleData();

private:
    QString m_host;
    quint16 m_port;
    QTcpSocket* m_pSocket;
};

#endif // SISCLIENT_H
