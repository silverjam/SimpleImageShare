#include "server.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisServer::SisServer(QHostAddress address/* = QHostAddress::Any*/, int port/* = 11507*/, QObject *parent/* = 0*/)
    : QObject(parent)
    , m_command(COMMAND_UNKNOWN)
{
    m_pServer = new QTcpServer(this);
    if ( ! m_pServer->listen(address, port) )
        qDebug("(%s:%d) server failed to listen", __FILE__, __LINE__);

    m_pSigMap = new QSignalMapper(this);

    // Wait for header first
    m_pool.setChunkSize(SisCommandParser::headerSize());

    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(handleConnection()));
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

    // Need to move most of this logic to the command parser class and just
    //  feed it the data and the parser sink.

    QByteArray buffer;

    SisCommandParser commands(this, this);
    m_pool.poolNewData(pSocket->readAll());

    if ( m_pool.isDataReady() && m_pool.chunkSize() == SisCommandParser::headerSize() )
    {
        m_pool.readPooledChunk(buffer);

        QDataStream ds(buffer);
        m_command = commands.readHeader(ds);

        qint64 size = commands.discoverSize(m_command);

        m_pool.setChunkSize(size);
    }

    if ( m_pool.isDataReady() )
    {
        buffer.clear();

        QDataStream ds(buffer);
        m_pool.readPooledChunk(buffer);

        if ( ! commands.parseOne(ds, m_command) )
        {
            qDebug("Failed to parse protocol buffer");
        }
    }

    emit dataProcessed();
}
