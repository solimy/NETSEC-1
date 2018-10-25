#ifndef PROTOCOLENUM_H
#define PROTOCOLENUM_H

enum class ProtocolEnum : int {
    UNKNOWN__RAW,
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
