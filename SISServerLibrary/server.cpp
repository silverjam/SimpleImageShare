#include "server.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisServer::SisServer(CommandSink* pSink, QHostAddress address/* = QHostAddress::Any*/, int port/* = 11507*/, QObject *parent/* = 0*/)
    : QObject(parent)
    , m_parser(pSink, this)
{
    m_pServer = new QTcpServer(this);
    if ( ! m_pServer->listen(address, port) )
        qDebug("(%s:%d) server failed to listen", __FILE__, __LINE__);

    m_pSigMap = new QSignalMapper(this);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    connect(&m_parser, SIGNAL(dataProcessed()), this, SIGNAL(dataProcessed()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int
SisServer::port() const
{
    return m_pServer->serverPort();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisServer::handleConnection()
{
    QTcpSocket* pSocket = m_pServer->nextPendingConnection();

    connect(pSocket, SIGNAL(readyRead()), m_pSigMap, SLOT(map()));

    m_pSigMap->setMapping(pSocket, pSocket);

    connect(m_pSigMap, SIGNAL(mapped(QObject*)), this, SLOT(handleData(QObject*)));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisServer::handleData(QObject* pObject)
{
    QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(pObject);
    if ( ! pSocket )
    {
        qDebug("non socket passed to handleData");
        return;
    }

    QByteArray readBuffer(pSocket->readAll());
    m_parser.processData(readBuffer);
}
