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

#include <iostream>
namespace DEBUG {
    class PacketDumper : public PcapFeeder::PcapFeedable {
    public:
        virtual void feed(const PcapPacket* p) {
            std::cout << "____" << std::endl;
            switch (p->protocol) {
            case ProtocolEnum::UNKNOWN:
                std::cout << "| UNKNOWN=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::ETHERNET:
                std::cout << "| ETHERNET=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::ARP:
                std::cout << "| ARP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::IP:
                std::cout << "| IP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::ICMP:
                std::cout << "| ICMP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::TCP:
                std::cout << "| TCP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::UDP:
                std::cout << "| UDP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::HTTP:
                std::cout << "| HTTP=" << (int)p->protocol << std::endl;
                break;
            case ProtocolEnum::DNS:
                std::cout << "| DNS=" << (int)p->protocol << std::endl;
                break;
            }
            std::cout << "|" << std::endl;
            std::cout << "| pcapHeader.ts_sec=" << p->raw->pcapHeader.ts_sec << std::endl;
            std::cout << "| pcapHeader.ts_usec=" << p->raw->pcapHeader.ts_usec << std::endl;
            std::cout << "| pcapHeader.incl_len=" << p->raw->pcapHeader.incl_len << std::endl;
            std::cout << "| pcapHeader.orig_len=" << p->raw->pcapHeader.orig_len << std::endl;
            std::cout << "|" << std::endl;
            std::cout << "| ehternetHeader.h_proto=" << ((EthernetRaw*)p->raw)->ehternetHeader.h_proto << std::endl;
            std::cout << "|___" << std::endl;
        }
    };
}


#endif // PCAPFEEDER_H
