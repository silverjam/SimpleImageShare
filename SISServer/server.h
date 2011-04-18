#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

class Server : public QObject
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
