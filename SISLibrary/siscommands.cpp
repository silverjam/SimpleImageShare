#include "siscommands.h"
#include "commanddata.h"

enum CommandCodes {
    COMMAND_PROTO_VERSION = 42,
    COMMAND_DISCOVERED_IMAGE_SETS
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisCommandParser::SisCommandParser(ICommandSink* pSink, QObject* parent/* = 0*/)
    : QObject(parent)
    , m_pSink(pSink)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
qint64
SisCommandParser::discoverSize(quint32 command)
{
    switch(command) {
    case COMMAND_PROTO_VERSION:
        return ProtocolVersion::size();
    case COMMAND_DISCOVERED_IMAGE_SETS:
        return DiscoveredImageSets::size();
    }

    return -1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
qint64
SisCommandParser::headerSize()
{
    return sizeof(quint32);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
quint32
SisCommandParser::readHeader(QDataStream& ds)
{
    quint32 header;
    ds >> header;

    return header;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommandParser::parseOne(QDataStream& ds, quint32 command)
{
    switch(command) {
    case COMMAND_PROTO_VERSION:
        return parse_ProtocolVersion(ds);
    case COMMAND_DISCOVERED_IMAGE_SETS:
        return parse_DiscoveredImageSets(ds);
    }

    return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommandBuilder::build_ProtocolVersion(QDataStream& ds)
{
    ProtocolVersion data;
    data.version = CURRENT_PROTOCOL_VERSION;

    ds << COMMAND_PROTO_VERSION;
    ds << data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommandParser::parse_ProtocolVersion(QDataStream& ds)
{
    ProtocolVersion info;
    ds >> info;

    m_pSink->handle_ProtocolVersion(info.version);

    return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommandBuilder::build_DiscoveredImageSets(QDataStream& ds, int count)
{
    DiscoveredImageSets data;
    data.count = count;

    ds << COMMAND_DISCOVERED_IMAGE_SETS;
    ds << data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommandParser::parse_DiscoveredImageSets(QDataStream& ds)
{
    DiscoveredImageSets data;
    ds >> data;

    m_pSink->handle_DiscoveredImageSets(data.count);

    return true;
}
