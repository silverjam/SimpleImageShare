#include "siscommands.h"

using namespace CommandInformation;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisCommands::SisCommands(QDataStream& dataStream, ICommandSink* pSink, QObject* parent/* = 0*/)
    : QObject(parent)
    , m_dataStream(dataStream)
    , m_pSink(pSink)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommands::parseOne()
{
    int command;
    m_dataStream >> command;

    switch(command) {
    case COMMAND_PROTO_VERSION:
        return parse_ProtocolVersion();
    case COMMAND_DISCOVER_IMAGE_SETS:
        return parse_DiscoverImageSets();
    }

    return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommands::build_ProtocolVersion()
{
    m_dataStream << COMMAND_PROTO_VERSION;
    m_dataStream << 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommands::parse_ProtocolVersion()
{
    ProtocolVersion info;
    m_dataStream >> info.version;

    m_pSink->handle_ProtocolVersion(info);

    return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommands::build_DiscoverImageSets(int count)
{
    m_dataStream << COMMAND_DISCOVER_IMAGE_SETS;
    m_dataStream << count;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommands::parse_DiscoverImageSets()
{
    DiscoverImageSets info;
    m_dataStream >> info.count;

    m_pSink->handle_DiscoverImageSets(info);

    return true;
}
