#ifndef ICOMMANDSINK_H
#define ICOMMANDSINK_H

namespace CommandInformation
{
    struct DiscoverImageSets
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
    virtual void handle_DiscoverImageSets(const CommandInformation::DiscoverImageSets&) = 0;
    virtual void handle_ProtocolVersion(const CommandInformation::ProtocolVersion&) = 0;
};

#endif // ICOMMANDSINK_H
