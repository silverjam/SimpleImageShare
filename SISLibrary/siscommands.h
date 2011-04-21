#ifndef SISCOMMANDS_H
#define SISCOMMANDS_H

#include <QtNetwork>

#include "icommandsink.h"

enum { CURRENT_PROTOCOL_VERSION = 20110418 };

class SisCommandParser : QObject
{
public:
    SisCommandParser(ICommandSink*, QObject* = 0);

    qint64 discoverSize(quint32 command);
    bool parseOne(QDataStream&, quint32 command);

    qint64 headerSize();
    quint32 readHeader(QDataStream& ds);

    bool parse_ProtocolVersion(QDataStream& ds);
    bool parse_DiscoveredImageSets(QDataStream& ds);

private:
    ICommandSink* m_pSink;
};

class SisCommandBuilder
{
public:
    static void build_ProtocolVersion(QDataStream&);
    static void build_DiscoveredImageSets(QDataStream&, int count);
};

#endif // SISCOMMANDS_H
