#ifndef SISCOMMANDS_H
#define SISCOMMANDS_H

#include <QtNetwork>

#include "icommandsink.h"

enum CommandCode {
    COMMAND_UNKNOWN = 0,
    COMMAND_PROTO_VERSION = 42,
    COMMAND_DISCOVERED_IMAGE_SETS
};

enum { CURRENT_PROTOCOL_VERSION = 20110418 };

class SisCommandParser : QObject
{
public:
    SisCommandParser(ICommandSink*, QObject* = 0);

    qint64 discoverSize(quint32 command);
    bool parseOne(QDataStream&, quint32 command);

    static qint64 headerSize();
    CommandCode readHeader(QDataStream& ds);

    bool parse_ProtocolVersion(QDataStream& ds);
    bool parse_DiscoveredImageSets(QDataStream& ds);

private:
    ICommandSink* m_pSink;
};

class SisCommandBuilder : public QObject
{
public:
    static void build_ProtocolVersion(QDataStream&);
    static void build_DiscoveredImageSets(QDataStream&, int count);

    inline void setBuffer(QBuffer* pds) { m_pbuf = pds; }
    inline QBuffer* buffer() { return m_pbuf; }

    virtual void send(const QBuffer&) = 0;

    inline void prepareBuffer()
    {
        m_pbuf->seek(0);
        m_pbuf->setData(0, 0);
    }

    inline void send_ProtocolVersion() {
        prepareBuffer();
        QDataStream ds(m_pbuf);
        build_ProtocolVersion(ds);
        send(*m_pbuf);
    }

private:
    QBuffer* m_pbuf;
};

#endif // SISCOMMANDS_H
