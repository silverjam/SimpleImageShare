#include "sisclient.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SISClient::SISClient(const QString& address, quint16 port)
    : m_host(address)
    , m_port(port)
    , m_pSocket(0)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISClient::connectToHost()
{
    m_pSocket = new QTcpSocket(this);
    m_pSocket->connectToHost(m_host, m_port);

    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(handleData()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISClient::handleData()
{
    QByteArray buffer = m_pSocket->readAll();
}
