#include "../pcapfeeder.h"

class Feedable : public PcapFeeder::PcapFeedable {
public:
    virtual void feed(const std::shared_ptr<PcapPacket> packet) {
        printf("%d\n", packet->protocol);
    }
};

class Feeder : public PcapFeeder {
public :
    void test() {
        std::shared_ptr<PcapPacket> pcap = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<PcapRaw>(0));
        pcap->protocol = ProtocolEnum::UNKNOWN;
        std::shared_ptr<PcapPacket> eth = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<EthernetRaw>(0));
        eth->protocol = ProtocolEnum::ETHERNET;
        std::shared_ptr<PcapPacket> arp = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<ARPRaw>(0));
        arp->protocol = ProtocolEnum::ARP;
        std::shared_ptr<PcapPacket> ip = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<IPRaw>(0));
        ip->protocol = ProtocolEnum::IP;
        std::shared_ptr<PcapPacket> icmp = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<ICMPRaw>(0));
        icmp->protocol = ProtocolEnum::ICMP;
        std::shared_ptr<PcapPacket> udp = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<UDPRaw>(0));
        udp->protocol = ProtocolEnum::UDP;
        std::shared_ptr<PcapPacket> dns = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<DNSRaw>(0));
        dns->protocol = ProtocolEnum::DNS;
        std::shared_ptr<PcapPacket> tcp = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<TCPRaw>(0));
        tcp->protocol = ProtocolEnum::TCP;
        std::shared_ptr<PcapPacket> http = std::make_shared<PcapPacket>(PcapPacket::forgeRaw<HTTPRaw>(0));
        http->protocol = ProtocolEnum::HTTP;
        feedSubscribers(pcap);
        feedSubscribers(eth);
        feedSubscribers(arp);
        feedSubscribers(ip);
        feedSubscribers(icmp);
        feedSubscribers(udp);
        feedSubscribers(dns);
        feedSubscribers(tcp);
        feedSubscribers(http);
    }
};

int main() {
    Feeder feeder;
    Feedable feedable;
    feedable.subscribeToFeeder(&feeder);
    feeder.test();
}
