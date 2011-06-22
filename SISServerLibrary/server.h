#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

#include "sislibrary.h"

class SisServer : public SisCommandBuilder
{
    Q_OBJECT
public:
    explicit SisServer(CommandSink* pSink, QHostAddress address = QHostAddress::Any, int port = 11507, QObject *parent = 0);

    int port() const;
    QList<QTcpSocket*> clients() const;

signals:
    void dataProcessed();
    void newClient();
    void lostClient();

public slots:
    void handleConnection();
    void handleData(QObject*);
    void handleDisconnect(QObject*);

private:
    // From SisCommandBuilder
    virtual void send(const QBuffer &, void *pvContext);

    QTcpServer* m_pServer;

    QSignalMapper* m_pSigMap;
    QSignalMapper* m_pSigMapDisco;

    SisCommandParser* m_pParser;

    QBuffer m_buffer;

    QMap<QTcpSocket*, QTcpSocket*> m_mapClients;
};

#endif // SERVER_H
