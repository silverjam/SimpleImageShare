#ifndef COMMANDDATA_H
#define COMMANDDATA_H

#include <QtCore>

struct ProtocolVersion {
    quint32 version;
    static qint64 size();
};

QDataStream& operator<< (QDataStream&, const ProtocolVersion&);
QDataStream& operator>> (QDataStream&, ProtocolVersion&);

struct DiscoveredImageSets {
    quint32 count;
    static qint64 size();
};

QDataStream& operator<< (QDataStream&, const DiscoveredImageSets&);
QDataStream& operator>> (QDataStream&, DiscoveredImageSets&);

#endif // COMMANDDATA_H
