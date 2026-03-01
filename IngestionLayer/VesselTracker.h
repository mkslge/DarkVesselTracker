//
// Created by Mark on 3/1/26.
//

#ifndef VESSELTRACKER_H
#define VESSELTRACKER_H
#include "VesselState.h"
#include "SpatialIndex.h"
#include <unordered_map>
#include <cstdint>

inline std::uint64_t DEFAULT_DARK_THRESHOLD = 1800;

class VesselTracker {
public:

    explicit VesselTracker(SpatialIndex &spatialIndex, std::uint64_t darkThresholdSeconds = DEFAULT_DARK_THRESHOLD);

    void updateVessel(PositionUpdate);
    void reviewDarkEvents(std::uint64_t);



private:
    std::unordered_map<std::uint32_t, VesselState> vessels_;
    std::uint64_t darkThreshold_;
    SpatialIndex spatialIndex_;

    void updateEvent(PositionUpdate update);
};



#endif //VESSELTRACKER_H
