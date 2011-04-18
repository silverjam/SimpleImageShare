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
        COMMAND_DISCOVER_IMAGE_SETS,
    };

    enum CommandVersions {
        VERSION_DISCOVER = 1,
        VERSION_VERSION = 1,
    };

    SisCommands(QDataStream&, ICommandSink*);

    void build_ProtocolVersion();
    bool parse_ProtocolVersion();

    void build_DiscoverImageSets();
    bool parse_DiscoverImageSets();

private:
    QDataStream& m_dataStream;
    ICommandSink* m_pSink;
};

#endif // SISCOMMANDS_H
