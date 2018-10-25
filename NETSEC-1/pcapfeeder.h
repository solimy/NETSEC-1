#ifndef PCAPFEEDER_H
#define PCAPFEEDER_H

#include <mutex>
#include <unordered_set>

#include "pcappacket.h"

class PcapFeeder {
public:
    class PcapFeedable {
        friend PcapFeeder;
    public:
        virtual ~PcapFeedable();

        /**
         * @brief subscribeToFeeder
         * @param feeder
         * @threadsafe
         */
        void subscribeToFeeder(PcapFeeder* feeder);

        /**
         * @brief unsubscribeToFeeder
         * @param feeder
         * @threadsafe
         */
        void unsubscribeToFeeder(PcapFeeder* feeder);

    protected:
        PcapFeedable();

        /**
         * @brief feed
         * @param packet
         * @threadsafe
         */
        virtual void feed(const PcapPacket* packet)=0;

    private:
        std::mutex feedSafety;

        /**
         * @brief safeFeed
         * @param packet
         * @threadsafe
         */
        void safeFeed(PcapPacket* packet);
    };

protected:

    /**
     * @brief feedSubscribers
     * @param packet
     * @threadsafe
     */
    void feedSubscribers(PcapPacket* packet);

private:
    std::mutex subscribersSafety;
    std::unordered_set<PcapFeedable*> subscribers;

    /**
     * @brief subscribe
     * @param feedable
     * @threadsafe
     */
    void subscribe(PcapFeedable* feedable);

    /**
     * @brief unsubscribe
     * @param feedable
     * @threadsafe
     */
    void unsubscribe(PcapFeedable* feedable);
};

#endif // PCAPFEEDER_H
