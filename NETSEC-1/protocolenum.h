#ifndef PROTOCOLENUM_H
#define PROTOCOLENUM_H

enum class ProtocolEnum : int {
    UNKNOWN,
    ETHERNET,
    ARP,
    ICMP,
    IP,
    TCP,
    UDP,
    HTTP,
    DNS
};

#endif // PROTOCOLENUM_H
