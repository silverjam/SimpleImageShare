#include "siscommands.h"

using namespace CommandInformation;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisCommands::SisCommands(QDataStream& dataStream, ICommandSink* pSink) :
    m_dataStream(dataStream),
    m_pSink(pSink)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommands::build_ProtocolVersion()
{
    m_dataStream << COMMAND_MARKER;
    m_dataStream << COMMAND_PROTO_VERSION;
    m_dataStream << VERSION_VERSION;
    m_dataStream << 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommands::parse_ProtocolVersion()
{
    ProtocolVersion info;

    int marker;
    m_dataStream >> marker;

    if ( marker != COMMAND_MARKER )
        qWarning("Marker didn't match: %1", marker);

    int command;
    m_dataStream >> command;

    if ( command != COMMAND_PROTO_VERSION )
        return false;

    int version;
    m_dataStream >> version;

    if ( VERSION_VERSION != version )
    {
        qWarning("Version didn't match: %1", version);
        return false;
    }

    m_dataStream >> info.version;

    m_pSink->handle_ProtocolVersion(info);

    return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommands::build_DiscoverImageSets()
{
    m_dataStream << COMMAND_MARKER;
    m_dataStream << COMMAND_DISCOVER_IMAGE_SETS;
    m_dataStream << VERSION_DISCOVER;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommands::parse_DiscoverImageSets()
{
    return false;
}
