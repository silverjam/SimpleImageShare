#include "server.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Server::Server(QHostAddress address/* = QHostAddress::Any*/, int port/* = 11507*/, QObject *parent/* = 0*/) :
    QObject(parent)
{
    m_pServer = new QTcpServer(this);
    m_pServer->listen(address, port);

    m_pSigMap = new QSignalMapper(this);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int
Server::port() const
{
    return m_pServer->serverPort();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
Server::handleConnection()
{
    QTcpSocket* pSocket = m_pServer->nextPendingConnection();

    connect(pSocket, SIGNAL(readyRead()), m_pSigMap, SLOT(map(QObject*)));

    m_pSigMap->setMapping(pSocket, pSocket);

    connect(m_pSigMap, SIGNAL(mapped(QObject*)), this, SLOT(handleData(QObject*)));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
Server::handleData(QObject* pObject)
{
    QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(pObject);
    if ( ! pSocket )
    {
        qDebug("non socket passed to handleData");
        return;
    }

    SisCommandParser commands((ICommandSink*)this, (QObject*)this);

    QDataStream ds;
    commands.parseOne(ds, 1);
}
