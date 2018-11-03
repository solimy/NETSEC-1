#ifndef PROTOCOLENUM_H
#define PROTOCOLENUM_H

#include <unordered_map>

enum class ProtocolEnum : int {
    UNKNOWN,
    ETHERNET, //IDENTIFIE
    ARP,
    ICMP, //IDENTIFIE
    IP, //IDENTIFIE
    TCP, //IDENTIFIE
    UDP, //IDENTIFIE
    HTTP, //IDENTIFIE
    DNS //IDENTIFIE
};

#endif // PROTOCOLENUM_H
