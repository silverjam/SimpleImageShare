#include "sisclient.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisClient::SisClient(const QString& address, quint16 port, CommandSink* pSink)
    : m_host(address)
    , m_port(port)
    , m_pSocket(0)
    , m_pParser(new SisCommandParser(pSink, this))
{
    m_buffer.open(QBuffer::ReadWrite);
    SisCommandBuilder::setBuffer(&m_buffer);

    connect(m_pParser, SIGNAL(dataProcessed()), this, SIGNAL(dataProcessed()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::connectToHost()
{
    m_pSocket = new QTcpSocket(this);
    m_pSocket->connectToHost(m_host, m_port);

    Q_ASSERT( connect(m_pSocket, SIGNAL(connected()), this, SIGNAL(connected())) );
    Q_ASSERT( connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(handleData())) );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::closeConnection()
{
    if ( ! m_pSocket )
    {
        qDebug("%s called without a socket (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
        return;
    }

    Q_ASSERT( disconnect(m_pSocket, SIGNAL(connected()), this, SIGNAL(connected())) );
    Q_ASSERT( disconnect(m_pSocket, SIGNAL(readyRead()), this, SLOT(handleData())) );

    m_pSocket->close();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::handleData()
{
    QByteArray buffer = m_pSocket->readAll();
    m_pParser->processData(buffer);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::send(const QBuffer& buf, void*)
{
    qint64 writeCount = m_pSocket->write(buf.data());
    Q_ASSERT( writeCount == buf.size() );
}
