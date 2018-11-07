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
#include <functional>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pcappacket.h"

class Filter {
public:
    enum OPT_TYPE {NO_OPT, EQUALS, NOTEQUALS};
    template<typename T>
    class FilterOpt {
        std::shared_ptr<T> value;
        OPT_TYPE type = OPT_TYPE::NO_OPT;
    public:
        void configure(OPT_TYPE optType, T value) {
            type = optType;
            this->value = std::make_shared<T>(value);
        }

        void reset() {
            type = OPT_TYPE::NO_OPT;
            value.reset();
        }

        bool isSet() {
            return type!=OPT_TYPE::NO_OPT && value.get() != nullptr;
        }

        bool filter(T value) {
            if (isSet())
                switch (type) {
                case OPT_TYPE::EQUALS:
                    return *this->value == value;
                case OPT_TYPE::NOTEQUALS:
                    return *this->value != value;
                default:
                    return true;
                }
            return true;
        }
    };

private:
    FilterOpt<ProtocolEnum> proto;
    FilterOpt<uint32_t> srcIp;
    FilterOpt<uint16_t> srcPort;
    FilterOpt<uint32_t> dstIp;
    FilterOpt<uint16_t> dstPort;

    const std::unordered_map<std::string, std::function<void(OPT_TYPE, const std::string&)> > filterFields = {
        {"proto", [this](OPT_TYPE optType, const std::string& value){
            if (value == "UNKNOWN") {
                this->proto.configure(optType, ProtocolEnum::UNKNOWN);
            } else if (value == "ETHERNET") {
                this->proto.configure(optType, ProtocolEnum::ETHERNET);
            } else if (value == "ARP") {
                this->proto.configure(optType, ProtocolEnum::ARP);
            } else if (value == "ICMP") {
                this->proto.configure(optType, ProtocolEnum::ICMP);
            } else if (value == "IP") {
                this->proto.configure(optType, ProtocolEnum::IP);
            } else if (value == "UDP") {
                this->proto.configure(optType, ProtocolEnum::UDP);
            } else if (value == "TCP") {
                this->proto.configure(optType, ProtocolEnum::TCP);
            } else if (value == "HTTP") {
                this->proto.configure(optType, ProtocolEnum::HTTP);
            } else if (value == "DNS") {
                this->proto.configure(optType, ProtocolEnum::DNS);
            }
            if (proto.isSet())
            printf("%s=%s\n", "proto", value.c_str());
        }},
        {"srcIp", [this](OPT_TYPE optType, const std::string& value){
            in_addr inaddr;
            if (inet_aton(value.c_str(), &inaddr) != 0) {
                srcIp.configure(optType, inaddr.s_addr);
            }
            if (srcIp.isSet())
            printf("%s=%s\n", "srcIp", value.c_str());
        }},
        {"dstIp", [this](OPT_TYPE optType, const std::string& value){
            in_addr inaddr;
            if (inet_aton(value.c_str(), &inaddr) != 0) {
                dstIp.configure(optType, inaddr.s_addr);
            }
            if (dstIp.isSet())
            printf("%s=%s\n", "dstIp", value.c_str());
        }},
        {"srcPort", [this](OPT_TYPE optType, const std::string& value){
            try {
                srcPort.configure(optType, htons(std::stoi(value.c_str())));
            } catch (std::invalid_argument& ia) {
            }
            if (srcPort.isSet())
            printf("%s=%s\n", "srcPort", value.c_str());
        }},
        {"dstPort", [this](OPT_TYPE optType, const std::string& value){
            try {
                dstPort.configure(optType, htons(std::stoi(value.c_str())));
            } catch (std::invalid_argument& ia) {
            }
            if (dstPort.isSet())
            printf("%s=%s\n", "dstPort", value.c_str());
        }}
    };

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}


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
            OPT_TYPE optType;
            if (s.find("==") != std::string::npos) {
                replaceAll(s, "==", " ");
                optType = OPT_TYPE::EQUALS;
            } else if (s.find("!=") != std::string::npos) {
                replaceAll(s, "!=", " ");
                optType = OPT_TYPE::NOTEQUALS;
            } else
                continue;
            std::stringstream sss(s);
            sss >> field;
            sss >> value;
            auto opt = filterFields.at(field);
            if (opt != nullptr)
                opt(optType, value);
        }
    }

    bool filter(const std::shared_ptr<PcapPacket>& packet) {
        bool isOk = true;
        isOk &= proto.filter(packet->protocol);
        iphdr ipHdr;
        switch (packet->protocol) {
        case ProtocolEnum::UNKNOWN:
        case ProtocolEnum::ETHERNET:
        case ProtocolEnum::ARP:
            return isOk;
        default:
            ipHdr = ((IPRaw*)packet->raw)->ipHeader;
            isOk &= srcIp.filter(ipHdr.saddr);
            isOk &= dstIp.filter(ipHdr.daddr);
            break;
        }
        _udphdr udpHdr;
        switch (packet->protocol) {
        case ProtocolEnum::IP:
        case ProtocolEnum::ICMP:
            return isOk;
        default:
            udpHdr = ((UDPRaw*)packet->raw)->udpHeader;
            isOk &= srcPort.filter(udpHdr.source);
            isOk &= dstPort.filter(udpHdr.dest);
            break;
        }
        return isOk;
    }
};

#endif // FILTER_H
