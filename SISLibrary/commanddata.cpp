#include "commanddata.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename T> qint64
GetSize()
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream ds(&buffer);

    T obj;
    ds << obj;

    return buffer.size();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
qint64
ProtocolVersion::size()
{
    return GetSize<ProtocolVersion>();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QDataStream&
operator<< (QDataStream& ds, const ProtocolVersion& o)
{
    return ds << o.version;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QDataStream&
operator>> (QDataStream& ds, ProtocolVersion& o)
{
    return ds >> o.version;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
qint64
DiscoveredImageSets::size()
{
    return GetSize<DiscoveredImageSets>();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QDataStream&
operator<< (QDataStream& ds, const DiscoveredImageSets& o)
{
    return ds << o.count;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QDataStream&
operator>> (QDataStream& ds, DiscoveredImageSets& o)
{
    return ds >> o.count;
}
