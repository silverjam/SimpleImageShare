#include "siscommands.h"
#include "commanddata.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisCommandParser::SisCommandParser(CommandSink* pSink, QObject* parent/* = 0*/)
    : QObject(parent)
    , m_pSink(pSink)
{
    m_pSink->setParent(this);

    // Wait for header first
    m_pool.setChunkSize(SisCommandParser::headerSize());
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
CommandCode
SisCommandParser::readHeader(QDataStream& ds)
{
    quint32 header;
    ds >> header;

    return (CommandCode)header;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommandParser::processData(const QByteArray& readBuffer)
{
    QByteArray buffer;
    m_pool.poolNewData(readBuffer);

    if (m_pool.isDataReady() && m_pool.chunkSize() == SisCommandParser::headerSize() )
    {
        m_pool.readPooledChunk(buffer);

        QDataStream ds(buffer);
        m_command = readHeader(ds);

        qint64 size = discoverSize(m_command);

        m_pool.setChunkSize(size);
    }

    if ( m_pool.isDataReady() )
    {
        buffer.clear();

        m_pool.readPooledChunk(buffer);
        QDataStream ds(buffer);

        if ( ! parseOne(ds, m_command) )
        {
            qDebug("Failed to parse protocol buffer");
        }
    }

    emit dataProcessed();
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
SisCommandBuilder::prepareBuffer()
{
    m_pbuf->seek(0);
    m_pbuf->close();
    m_pbuf->setData(0, 0);
    m_pbuf->open(QIODevice::ReadWrite);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
SisCommandParser::parse_ProtocolVersion(QDataStream& ds)
{
    ProtocolVersion info;
    ds >> info;

    m_pSink->emit_ProtocolVersion(info.version);

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

    m_pSink->emit_DiscoverImageSets(data.count);

    return true;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SisCommandBuilder::SisCommandBuilder(QObject* parent /*= 0*/)
    : QObject(parent)
{
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SisCommandBuilder::send_ProtocolVersion(void* pvContext)
{
    prepareBuffer();
    QDataStream ds(m_pbuf);
    build_ProtocolVersion(ds);
    send(*m_pbuf, pvContext);
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
