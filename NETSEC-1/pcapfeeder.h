#ifndef PCAPFEEDER_H
#define PCAPFEEDER_H

#include "mutex"
#include "unordered_set"

#include "pcappacket.h"

class PcapFeeder {
public:
    class PcapFeedable {
        friend PcapFeeder;
    public:
        virtual ~PcapFeedable();

        /**///Thread safe
        /**/virtual void feed(PcapPacket* packet);
        /**/void subscribeToFeeder(PcapFeeder* feeder);
        /**/void unsubscribeToFeeder(PcapFeeder* feeder);

    private:
        std::mutex feedSafety;

        void safeFeed(PcapPacket* packet);
    };

protected:
    void feedSubscribers(PcapPacket* packet);

private:
    std::mutex subscribersSafety;
    std::unordered_set<PcapFeedable*> subscribers;

    void subscribe(PcapFeedable* feedable);
    void unsubscribe(PcapFeedable* feedable);
};

#endif // PCAPFEEDER_H
