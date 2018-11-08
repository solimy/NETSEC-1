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

void PcapFeeder::PcapFeedable::safeFeed(std::shared_ptr<PcapPacket> packet) {
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

void PcapFeeder::forceFeed(PcapFeedable* victim, std::shared_ptr<PcapPacket> packet) {
    victim->safeFeed(packet);
}

void PcapFeeder::feedSubscribers(std::shared_ptr<PcapPacket> packet) {
    std::lock_guard<std::mutex> lock(subscribersSafety);
    std::unordered_set<PcapFeedable*> deads;

    for (auto subscriber : subscribers) {
        try {
            subscriber->safeFeed(packet);
        } catch (std::exception& e) {
            deads.insert(subscriber);
        }
    }
    for (auto dead : deads)
        subscribers.erase(dead);
}
