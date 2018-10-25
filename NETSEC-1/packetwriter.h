#ifndef PACKETWRITER_H
#define PACKETWRITER_H

#include <string>

#include "pcapfeeder.h"

class PacketWriter : public PcapFeeder::PcapFeedable
{
public:
    PacketWriter();
    ~PacketWriter();

    /**
     * @brief startWriting
     * @details setup raw socket ready for write, store fd in "socket"
     */
    void startWriting();

    /**
     * @brief startWriting
     * @param file
     * @details setup Pcap compatible file ready for write, store fd in "socket"
     */
    void startWriting(std::string const& file);

    void stopWriting();

protected:
    virtual void feed(const PcapPacket* packet);

private:
    int socket = -1;
};

#endif // PACKETWRITER_H
