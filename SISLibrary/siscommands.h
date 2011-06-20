#ifndef SISCOMMANDS_H
#define SISCOMMANDS_H

#include <QtNetwork>

#include "commandsink.h"
#include "datapool.h"

enum CommandCode {
    COMMAND_UNKNOWN = 0,
    COMMAND_PROTO_VERSION = 42,
    COMMAND_DISCOVERED_IMAGE_SETS
};

enum { CURRENT_PROTOCOL_VERSION = 20110418 };

class SisCommandParser : public QObject
{
    Q_OBJECT

public:
    SisCommandParser(CommandSink*, QObject* = 0);
    inline virtual ~SisCommandParser() { }

    qint64 discoverSize(quint32 command);
    bool parseOne(QDataStream&, quint32 command);

    static qint64 headerSize();
    CommandCode readHeader(QDataStream& ds);

    void processData(const QByteArray&);

    bool parse_ProtocolVersion(QDataStream& ds);
    bool parse_DiscoveredImageSets(QDataStream& ds);

signals:
    void dataProcessed();

private:
    DataPool m_pool;
    CommandCode m_command;

    CommandSink* m_pSink;
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
        m_pbuf->close();
        m_pbuf->setData(0, 0);
        m_pbuf->open(QIODevice::ReadWrite);
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
