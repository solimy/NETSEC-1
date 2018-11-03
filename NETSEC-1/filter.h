#ifndef FILTER_H
#define FILTER_H

#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pcappacket.h"

class Filter {
    std::shared_ptr<ProtocolEnum> proto;
    std::shared_ptr<uint32_t> srcIp;
    std::shared_ptr<uint16_t> srcPort;
    std::shared_ptr<uint32_t> dstIp;
    std::shared_ptr<uint16_t> dstPort;

    const std::unordered_map<std::string, std::function<void(const std::string&)> > filterFields = {
        {"proto", [this](const std::string& value){
            if (value == "UNKNOWN") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::UNKNOWN);
            } else if (value == "ETHERNET") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::ETHERNET);
            } else if (value == "ARP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::ARP);
            } else if (value == "ICMP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::ICMP);
            } else if (value == "IP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::IP);
            } else if (value == "UDP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::UDP);
            } else if (value == "TCP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::TCP);
            } else if (value == "HTTP") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::HTTP);
            } else if (value == "DNS") {
                this->proto = std::make_shared<ProtocolEnum>(ProtocolEnum::DNS);
            }
            if (proto.get() != nullptr)
            printf("%s=%s\n", "proto", value.c_str());
        }},
        {"srcIp", [this](const std::string& value){
            in_addr inaddr;
            if (inet_aton(value.c_str(), &inaddr) != 0) {
                srcIp = std::make_shared<uint32_t>(inaddr.s_addr);
            }
            if (srcIp.get() != nullptr)
            printf("%s=%s\n", "srcIp", value.c_str());
        }},
        {"dstIp", [this](const std::string& value){
            in_addr inaddr;
            if (inet_aton(value.c_str(), &inaddr) != 0) {
                dstIp = std::make_shared<uint32_t>(inaddr.s_addr);
            }
            if (dstIp.get() != nullptr)
            printf("%s=%s\n", "dstIp", value.c_str());
        }},
        {"srcPort", [this](const std::string& value){
            try {
                srcPort = std::make_shared<uint16_t>(htons(std::stoi(value.c_str())));
            } catch (std::invalid_argument& ia) {
            }
            if (srcPort.get() != nullptr)
            printf("%s=%s\n", "srcPort", value.c_str());
        }},
        {"dstPort", [this](const std::string& value){
            try {
            dstPort = std::make_shared<uint16_t>(htons(std::stoi(value.c_str())));
            } catch (std::invalid_argument& ia) {
            }
            if (dstPort.get() != nullptr)
            printf("%s=%s\n", "dstPort", value.c_str());
        }}
    };

public:
    Filter() {
    }

    ~Filter() {}

    void setFilters(const std::string& filters) {
        proto.reset();
        srcIp.reset();
        srcPort.reset();
        dstIp.reset();
        dstPort.reset();

        std::stringstream ss;
        ss.str(filters);
        while (ss.good()) {
            std::string s, field, value;
            ss >> s;
            if (s.find('=') == std::string::npos)
                continue;
            std::replace(s.begin(), s.end(), '=', ' ');
            std::stringstream sss(s);
            sss >> field;
            sss >> value;
            auto opt = filterFields.at(field);
            if (opt != nullptr)
                opt(value);
        }
    }

    bool filter(const std::shared_ptr<PcapPacket>& packet) {
        bool isOk = true;
        if (proto.get() != nullptr) {
            isOk &= packet->protocol == *proto;
        }
    iphdr ipHdr;
    if (srcIp.get() != nullptr) {
            switch (packet->protocol) {
            case ProtocolEnum::UNKNOWN:
            case ProtocolEnum::ETHERNET:
            case ProtocolEnum::ARP:
                return false;
            default:
                ipHdr = ((IPRaw*)packet->raw)->ipHeader;
                isOk &= ipHdr.saddr == *srcIp;
                break;
            }
        }
        if (dstIp.get() != nullptr) {
            switch (packet->protocol) {
            case ProtocolEnum::UNKNOWN:
            case ProtocolEnum::ETHERNET:
            case ProtocolEnum::ARP:
                return false;
            default:
                ipHdr = ((IPRaw*)packet->raw)->ipHeader;
                isOk &= ipHdr.daddr == *dstIp;
                break;
            }
        }
        udphdr udpHdr;
        if (srcPort.get() != nullptr) {
            switch (packet->protocol) {
            case ProtocolEnum::IP:
            case ProtocolEnum::ICMP:
                return false;
            default:
                udpHdr = ((UDPRaw*)packet->raw)->udpHeader;
                isOk &= udpHdr.source == *srcPort;
                break;
            }
        }
        if (dstPort.get() != nullptr) {
            switch (packet->protocol) {
            case ProtocolEnum::IP:
            case ProtocolEnum::ICMP:
                return false;
            default:
                udpHdr = ((UDPRaw*)packet->raw)->udpHeader;
                isOk &= udpHdr.dest == *dstPort;
                break;
            }
        }
        return isOk;
    }
};

#endif // FILTER_H
