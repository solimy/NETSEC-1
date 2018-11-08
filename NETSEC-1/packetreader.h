#ifndef PACKETREADER_H
#define PACKETREADER_H

#include <thread>
#include <string>
#include <atomic>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pcapfeeder.h"
#include "pcap.h"

class PacketReader : public PcapFeeder
{
public:
    PacketReader();
    ~PacketReader();

    /**
     * @brief startCapture
     * @details capture from network
     */
    void startCapture();

    /**
     * @brief startCapture
     * @param file
     * @details capture from given file
     *
     */
    void startCapture(std::string const& file);

    /**
     * @brief stopCapture
     * @details stop the capture
     */
    void stopCapture();

private:
    std::mutex routineSafety;
    std::atomic_bool running;
    std::thread* routine;

    /**
     * @brief captureFromNetwork
     * @details capture from the network. Stop when "running == false".
     * @details MUST call "this->feedSubscribers(PcapPacket& packet)" for every packet read
     *
     */
    void captureFromNetwork();

    /**
     * @brief captureFromFile
     * @param file
     * @details capture from "file". Stops when at "end of file" or when "running == false"
     * @details MUST call "this->feedSubscribers(PcapPacket& packet)" for every packet read
     */
    void captureFromFile(std::string const& file);
};

#endif // PACKETREADER_H
