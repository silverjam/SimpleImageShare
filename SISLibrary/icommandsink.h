#ifndef ICOMMANDSINK_H
#define ICOMMANDSINK_H

class ICommandSink
{
public:
    virtual void incoming_DiscoverImageSets(int count) = 0;
    virtual void incoming_ProtocolVersion(int version) = 0;
};

#endif // ICOMMANDSINK_H
