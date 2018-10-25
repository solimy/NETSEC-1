#ifndef PACKETREADER_H
#define PACKETREADER_H

#include <thread>
#include <string>

#include "pcapfeeder.h"

class PacketReader : public PcapFeeder
{
public:
    PacketReader();

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
    bool running = false;
    std::thread* routine = nullptr;

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
