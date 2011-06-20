#ifndef COMMANDSINK_H
#define COMMANDSINK_H

#include <QObject>

class CommandSink : public QObject
{
    Q_OBJECT

public:
    explicit CommandSink(QObject *parent = 0);

public:
    inline void emit_DiscoverImageSets(int count) { emit incoming_DiscoverImageSets(count); }
    inline void emit_ProtocolVersion(int version) { emit incoming_ProtocolVersion(version); }

signals:
    void incoming_DiscoverImageSets(int count);
    void incoming_ProtocolVersion(int version);

public slots:

};

#endif // COMMANDSINK_H
