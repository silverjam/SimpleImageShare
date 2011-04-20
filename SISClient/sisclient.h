#ifndef SISCLIENT_H
#define SISCLIENT_H

#include <QtNetwork>

class SISClient
    : QObject
{
    Q_OBJECT

public:
    SISClient(const QString& address, int port);
};

#endif // SISCLIENT_H
