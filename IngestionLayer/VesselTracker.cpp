//
// Created by Mark on 3/1/26.
//

#include "VesselTracker.h"
// Created by Mark on 3/1/26.

#include "VesselTracker.h"
#include <iostream>

#include "SpatialIndex.h"

// ---------------------------------------------------------------------------
// Assumed layout of VesselState.h:
//
//   struct VesselState {
//       uint64_t last_timestamp = 0;
//       double   last_lat       = 0.0;
//       double   last_lon       = 0.0;
//       double   last_sog       = 0.0;
//       bool     in_dark_event  = false;
//   };
//
//   struct PositionUpdate {
//       uint32_t mmsi;
//       uint64_t timestamp;   // Unix epoch seconds
//       double   lat;
//       double   lon;
//       double   sog;         // Speed over ground (knots)
//   };
// ---------------------------------------------------------------------------


VesselTracker::VesselTracker(SpatialIndex &spatialIndex, std::uint64_t darkThresholdSeconds)
    : spatialIndex_(spatialIndex), darkThreshold_(darkThresholdSeconds)
{}

void VesselTracker::updateVessel(const PositionUpdate update) {

    auto it = vessels_.find(update.mmsi);

    if (it == vessels_.end()) {
        // First time we've seen this vessel — just record its state
        VesselState state;
        state.last_timestamp = update.timestamp;
        state.last_lat       = update.lat;
        state.last_lon       = update.lon;
        state.last_sog       = update.sog;
        state.in_dark_event  = false;
        vessels_.emplace(update.mmsi, state);
        return;
    }

    VesselState& state = it->second;
    if (update.timestamp < state.last_timestamp) {

        return; //if we have out of order data we do not want to flag a dark event
    }
    uint64_t delta_time = update.timestamp - state.last_timestamp;

    if (state.in_dark_event) {
        // Vessel has reappeared — end the dark event
        std::cout << "[DARK END]   MMSI=" << update.mmsi
                  << "  silent_for=" << delta_time << "s"
                  << "  reappeared_at=" << update.timestamp << "\n";
        state.in_dark_event = false;

    } else if (delta_time > darkThreshold_ && !spatialIndex_.isNearPort(update.lat, update.lon, 10000)) {
        // Gap exceeds threshold — start a dark event
        std::cout << "[DARK START] MMSI=" << update.mmsi
                  << "  last_seen=" << state.last_timestamp
                  << "  gap=" << delta_time << "s\n";
        state.in_dark_event = true;
    }

    // Always update the stored state with the latest ping
    state.last_timestamp = update.timestamp;
    state.last_lat        = update.lat;
    state.last_lon        = update.lon;
    state.last_sog        = update.sog;
}

void VesselTracker::reviewDarkEvents(uint64_t ts) {
    // This method is called periodically (e.g. on a timer) with the current
    // wall-clock time so we can catch vessels that simply stopped transmitting
    // rather than waiting for a future update to trigger the dark-start logic.
    //
    // Typically you would pass the current timestamp in; here we accept it
    // via a parameter-less signature as required by the original header, so
    // callers should invoke it regularly from a thread/timer.
    //
    // NOTE: If you can change the signature, prefer:
    //   void reviewDarkEvents(uint64_t currentTimestamp);
    int darkEventCount = 0;
    std::cout << "[REVIEW] Checking " << vessels_.size() << " vessel(s) for dark events.\n";

    for (auto& [mmsi, state] : vessels_) {
        if (state.in_dark_event) {
            darkEventCount++;
            std::cout << "  MMSI=" << mmsi
                      << " is currently DARK (last seen "
            << state.last_timestamp << " at "  << state.last_lat << ", " << state.last_lon <<  ")\n";
        }

    }
    std::cout << "Total Dark Events: " << darkEventCount << std::endl;
}


