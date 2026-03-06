//
// Created by Mark on 3/1/26.
//

#include "VesselTracker.h"
// Created by Mark on 3/1/26.

#include "VesselTracker.h"
#include <iostream>
#include <utility>

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


VesselTracker::VesselTracker(SpatialIndex spatialIndex, std::uint64_t darkThresholdSeconds)
    : spatialIndex_(std::move(spatialIndex)), darkThreshold_(darkThresholdSeconds) {
    running = true;
}

void VesselTracker::updateVessel(const PositionUpdate& update) {
    std::lock_guard<std::mutex> lg(vessels_mutex_);
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
        /*std::cout << "[DARK END]   MMSI=" << update.mmsi
                  << "  silent_for=" << delta_time << "s"
                  << "  reappeared_at=" << update.timestamp << "\n";*/
        state.in_dark_event = false;

    } else if (delta_time > darkThreshold_ && !spatialIndex_.isNearPort(update.lat, update.lon, 10000)) {
        // Gap exceeds threshold — start a dark event
        /*std::cout << "[DARK START] MMSI=" << update.mmsi
                  << "  last_seen=" << state.last_timestamp
                  << "  gap=" << delta_time << "s\n";*/
        state.in_dark_event = true;
    }

    // Always update the stored state with the latest ping
    state.prev_lat = state.last_lat;
    state.prev_lon = state.last_lon;

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
    // Typically, you would pass the current timestamp in; here we accept it
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


void VesselTracker::darkEventScanner(uint64_t current_ts)
{
    std::lock_guard<std::mutex> lock(vessels_mutex_);

    for (auto& [mmsi, state] : vessels_) {

        if (state.in_dark_event)
            continue;

        uint64_t delta = current_ts - state.last_timestamp;

        if (delta > darkThreshold_) {

            DarkEvent ev;
            ev.mmsi = mmsi;
            ev.start_time = state.last_timestamp;
            ev.last_lat = state.last_lat;
            ev.last_lon = state.last_lon;
            ev.active = true;

            state.in_dark_event = true;

            queue_.push(ev);
        }
    }
}


void VesselTracker::eventProcessor()
{
    while (running.load()) {

        DarkEvent ev;

        if (queue_.try_pop(ev)) {
            std::cout << "Processing event in queue..." << std::endl;
            std::cout << "[DARK EVENT]\n";
            std::cout << "MMSI: " << ev.mmsi << "\n";
            std::cout << "Started: " << ev.start_time << "\n";
            std::cout << "Location: "
                      << ev.last_lat << ", "
                      << ev.last_lon << "\n\n";
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    }
}


void VesselTracker::monitorLoop() {
    uint64_t now = latest_ts_.load();
    while (running.load()) {
        std::cout << "In monitor loop, calling darkEventScanner..." << std::endl;
        darkEventScanner(now);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void VesselTracker::stop() {
    running = false;
}
