#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

#include "sislibrary.h"

class Server : public QObject, public ICommandSink
{
    Q_OBJECT
public:
    explicit Server(QHostAddress address = QHostAddress::Any, int port = 11507, QObject *parent = 0);

    int port() const;

signals:

public slots:
    void handleConnection();
    void handleData(QObject*);

private:
    inline virtual void handle_ProtocolVersion(quint32) { }
    inline virtual void handle_DiscoveredImageSets(quint32) { }

    QTcpServer* m_pServer;
    QSignalMapper* m_pSigMap;
};

#endif // SERVER_H
