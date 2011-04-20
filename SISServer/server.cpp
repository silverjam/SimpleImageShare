#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    m_pServer = new QTcpServer(this);
    m_pServer->listen(QHostAddress::Any, 11507);

    m_pSigMap = new QSignalMapper(this);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection()));
}

void
Server::handleConnection()
{
    QTcpSocket* pSocket = m_pServer->nextPendingConnection();

    connect(pSocket, SIGNAL(readyRead()), m_pSigMap, SLOT(map(QObject*)));

    m_pSigMap->setMapping(pSocket, pSocket);

    connect(m_pSigMap, SIGNAL(mapped(QObject*)), this, SLOT(handleData(QObject*)));
}

void
Server::handleData(QObject* pObject)
{
    QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(pObject);
    if ( ! pSocket )
    {
        qDebug("non socket passed to handleData");
        return;
    }

    SisCommands commands((QDataStream&)*(QDataStream*)(void*)0, (ICommandSink*)this, (QObject*)this);
    commands.parseOne();
}
