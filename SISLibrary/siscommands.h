#ifndef SISCOMMANDS_H
#define SISCOMMANDS_H

#include <QDataStream>

#include "icommandsink.h"

class SisCommands
{
public:
    enum CommandMarker {
        COMMAND_MARKER = 310953829,
    };

    enum CommandCodes {
        COMMAND_PROTO_VERSION = 42,
        COMMAND_DISCOVERED_IMAGE_SETS,
    };

    SisCommands(QDataStream&, ICommandSink*);

    bool parseOne();

    void build_ProtocolVersion();
    bool parse_ProtocolVersion();

    void build_DiscoveredImageSets(int count);
    bool parse_DiscoveredImageSets();

private:
    QDataStream& m_dataStream;
    ICommandSink* m_pSink;
};

#endif // SISCOMMANDS_H
