#ifndef PACKETWRITER_H
#define PACKETWRITER_H

#define NETWORK 0
#define FILE 1

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pcap.h>

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
    int type = -1;
    int fd[2] = {-1};
};

#endif // PACKETWRITER_H
