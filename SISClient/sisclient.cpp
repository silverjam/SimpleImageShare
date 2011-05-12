#include "sisclient.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisClient::SisClient(const QString& address, quint16 port, ICommandSink* pSink)
    : m_host(address)
    , m_port(port)
    , m_pSink(pSink)
    , m_pSocket(0)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::connectToHost()
{
    m_pSocket = new QTcpSocket(this);
    m_pSocket->connectToHost(m_host, m_port);

    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(handleData()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::handleData()
{
    QByteArray buffer = m_pSocket->readAll();
    qDebug("%s", buffer.data());
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisClient::send(const QBuffer& buf)
{
    m_pSocket->write(buf.data());
}
