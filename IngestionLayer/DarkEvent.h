//
// Created by Mark on 3/5/26.
//

#ifndef DARKEVENT_H
#define DARKEVENT_H
struct DarkEvent {
    uint64_t mmsi;

    uint64_t start_time;
    uint64_t end_time;

    double last_lat;
    double last_lon;

    bool active;
};
#endif //DARKEVENT_H
