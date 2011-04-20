#include "sisclient.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SISClient::SISClient(const QString& address, int port)
{
    QTcpSocket* pSocket = new QTcpSocket(this);
    //pSocket->connectToHost();
}
