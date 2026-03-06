//
// Created by Mark on 3/1/26.
//

#ifndef VESSELTRACKER_H
#define VESSELTRACKER_H
#include "VesselState.h"
#include "SpatialIndex.h"
#include <unordered_map>
#include <cstdint>
#include <mutex>
#include <thread>
#include "EventQueue/ConcurrentQueue.h"
#include "DarkEvent.h"
inline std::uint64_t DEFAULT_DARK_THRESHOLD = 1800;

class VesselTracker {
public:

    explicit VesselTracker(SpatialIndex spatialIndex, std::uint64_t darkThresholdSeconds = DEFAULT_DARK_THRESHOLD);

    void updateVessel(const PositionUpdate&);
    void reviewDarkEvents(std::uint64_t);
    void stop();


    void darkEventScanner(uint64_t timestamp);
    [[noreturn]] void eventProcessor();
    [[noreturn]] void monitorLoop();


private:
    std::unordered_map<std::uint32_t, VesselState> vessels_;
    std::uint64_t darkThreshold_;
    SpatialIndex spatialIndex_;
    std::mutex vessels_mutex_;
    ConcurrentQueue<DarkEvent> queue_;
    std::atomic<bool> running;

    std::atomic<uint64_t> latest_ts_{0};


};



#endif //VESSELTRACKER_H
