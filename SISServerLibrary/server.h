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

signals:
    void dataProcessed();

public slots:
    void handleConnection();
    void handleData(QObject*);

private:
    // From SisCommandBuilder
    virtual void send(const QBuffer &, void *pvContext);

    QTcpServer* m_pServer;
    QSignalMapper* m_pSigMap;

    SisCommandParser* m_pParser;

    QBuffer m_buffer;
};

#endif // SERVER_H
