#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

#include "sislibrary.h"

class Server : public QObject, public ICommandSink
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:

public slots:
    void handleConnection();
    void handleData(QObject*);

private:
    QTcpServer* m_pServer;
    QSignalMapper* m_pSigMap;
};

#endif // SERVER_H
