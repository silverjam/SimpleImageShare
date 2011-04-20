#ifndef ICOMMANDSINK_H
#define ICOMMANDSINK_H

namespace CommandInformation
{
    struct DiscoveredImageSets
    {
        int count;
    };

    struct ProtocolVersion
    {
        int version;
    };
}

class ICommandSink
{
public:
    virtual void handle_DiscoveredImageSets(const CommandInformation::DiscoveredImageSets&) = 0;
    virtual void handle_ProtocolVersion(const CommandInformation::ProtocolVersion&) = 0;
};

#endif // ICOMMANDSINK_H
