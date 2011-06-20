#include "server.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisServer::SisServer(CommandSink* pSink, QHostAddress address/* = QHostAddress::Any*/, int port/* = 11507*/, QObject *parent/* = 0*/)
    : SisCommandBuilder(parent)
    , m_pParser(new SisCommandParser(pSink, this))
{
    m_pServer = new QTcpServer(this);
    if ( ! m_pServer->listen(address, port) )
        qDebug("(%s:%d) server failed to listen", __FILE__, __LINE__);

    m_buffer.open(QBuffer::ReadWrite);
    SisCommandBuilder::setBuffer(&m_buffer);

    m_pSigMap = new QSignalMapper(this);

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    connect(m_pParser, SIGNAL(dataProcessed()), this, SIGNAL(dataProcessed()));
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
        qDebug("non socket passed to handleData (%s:%d)", __FILE__, __LINE__);
        return;
    }

    QByteArray readBuffer(pSocket->readAll());
    m_pParser->processData(readBuffer);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisServer::send(const QBuffer& buffer, void* pvContext)
{
    QTcpSocket* pSocket = reinterpret_cast<QTcpSocket*>(pvContext);
    if ( ! pSocket )
    {
        qDebug("non socket passed to send (%s:%d)", __FILE__, __LINE__);
        return;
    }

    qint64 writeCount = pSocket->write(buffer.data());

    Q_ASSERT( pSocket->isReadable() );
    Q_ASSERT( pSocket->isWritable() );
    Q_ASSERT( pSocket->isOpen() );
    Q_ASSERT( writeCount == buffer.size() );
}
