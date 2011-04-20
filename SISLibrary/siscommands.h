#ifndef SISCOMMANDS_H
#define SISCOMMANDS_H

#include <QDataStream>

#include "icommandsink.h"

class SisCommands : QObject
{
public:
    enum CommandCodes {
        COMMAND_PROTO_VERSION = 42,
        COMMAND_DISCOVER_IMAGE_SETS
    };

    SisCommands(QDataStream&, ICommandSink*, QObject* = 0);

    bool parseOne();

    void build_ProtocolVersion();
    bool parse_ProtocolVersion();

    void build_DiscoverImageSets(int count);
    bool parse_DiscoverImageSets();

private:
    QDataStream& m_dataStream;
    ICommandSink* m_pSink;
};

#endif // SISCOMMANDS_H
