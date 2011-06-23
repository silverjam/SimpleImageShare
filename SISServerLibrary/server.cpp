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
    m_pSigMapDisco = new QSignalMapper(this);

    VERIFY( connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection())) );
    VERIFY( connect(m_pParser, SIGNAL(dataProcessed()), this, SIGNAL(dataProcessed())) );

    VERIFY( connect(m_pSigMap, SIGNAL(mapped(QObject*)), this, SLOT(handleData(QObject*))) );
    VERIFY( connect(m_pSigMapDisco, SIGNAL(mapped(QObject*)), this, SLOT(handleDisconnect(QObject*))) );

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int
SisServer::port() const
{
    return m_pServer->serverPort();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QList<QTcpSocket*>
SisServer::clients() const
{
    return m_mapClients.values();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisServer::handleConnection()
{
    QTcpSocket* pSocket = m_pServer->nextPendingConnection();

    VERIFY( connect(pSocket, SIGNAL(readyRead()), m_pSigMap, SLOT(map())) );
    VERIFY( connect(pSocket, SIGNAL(disconnected()), m_pSigMapDisco, SLOT(map())) );

    m_pSigMap->setMapping(pSocket, pSocket);
    m_pSigMapDisco->setMapping(pSocket, pSocket);

    m_mapClients.insert(pSocket, pSocket);

    emit newClient();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisServer::handleDisconnect(QObject* pObject)
{
    QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(pObject);
    if ( ! pSocket )
    {
        qDebug("non socket passed to handleDisconnect (%s:%d)", __FILE__, __LINE__);
        return;
    }

    m_mapClients.remove(pSocket);

    VERIFY( disconnect(pSocket, SIGNAL(readyRead()), m_pSigMap, SLOT(map())) );
    VERIFY( disconnect(pSocket, SIGNAL(disconnected()), m_pSigMapDisco, SLOT(map())) );

    m_pSigMap->removeMappings(pSocket);
    pSocket->deleteLater();

    emit lostClient();
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
