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
    void closeConnection();

    inline QTcpSocket& socket() { return *m_pSocket; }

signals:
    void connected();
    void dataProcessed();

private slots:
    void handleData();

private:
    // From SisCommandBuilder
    virtual void send(const QBuffer& ds, void* pvContext);

    QString m_host;
    quint16 m_port;
    QTcpSocket* m_pSocket;
    QBuffer m_buffer;
    SisCommandParser* m_pParser;
};

#endif // SISCLIENT_H
