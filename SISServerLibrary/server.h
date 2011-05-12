#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

#include "sislibrary.h"

class SisServer : public QObject, public ICommandSink
{
    Q_OBJECT
public:
    explicit SisServer(QHostAddress address = QHostAddress::Any, int port = 11507, QObject *parent = 0);

    int port() const;

signals:

public slots:
    void handleConnection();
    void handleData(QObject*);

private:
    inline virtual void incoming_ProtocolVersion(int) { }
    inline virtual void incoming_DiscoverImageSets(int) { }

    QTcpServer* m_pServer;
    QSignalMapper* m_pSigMap;
};

#endif // SERVER_H
