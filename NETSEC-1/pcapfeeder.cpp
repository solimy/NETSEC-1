#include "pcapfeeder.h"

PcapFeeder::PcapFeedable::PcapFeedable() {
}

PcapFeeder::PcapFeedable::~PcapFeedable() {
}

void PcapFeeder::PcapFeedable::subscribeToFeeder(PcapFeeder* feeder) {
    feeder->subscribe(this);
}

void PcapFeeder::PcapFeedable::unsubscribeToFeeder(PcapFeeder* feeder) {
    feeder->unsubscribe(this);
}

void PcapFeeder::PcapFeedable::safeFeed(PcapPacket* packet) {
    std::lock_guard<std::mutex> lock(feedSafety);
    feed(packet);
}

void PcapFeeder::subscribe(PcapFeedable* feedable) {
    std::lock_guard<std::mutex> lock(subscribersSafety);
    subscribers.insert(feedable);
}

void PcapFeeder::unsubscribe(PcapFeedable* feedable) {
    std::lock_guard<std::mutex> lock(subscribersSafety);
    subscribers.erase(feedable);
}


void PcapFeeder::feedSubscribers(PcapPacket* packet) {
    std::lock_guard<std::mutex> lock(subscribersSafety);
    for (auto subscriber : subscribers)
        subscriber->safeFeed(packet);
}
