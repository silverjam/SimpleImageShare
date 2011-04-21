#ifndef ICOMMANDSINK_H
#define ICOMMANDSINK_H

class ICommandSink
{
public:
    virtual void handle_DiscoveredImageSets(int count) = 0;
    virtual void handle_ProtocolVersion(int version) = 0;
};

#endif // ICOMMANDSINK_H
