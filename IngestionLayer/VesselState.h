//
// Created by Mark on 3/1/26.
//

#ifndef VESSELSTATE_H
#define VESSELSTATE_H

#include "../DataSource/PositionUpdate.h"



struct VesselState {
    uint64_t last_timestamp;
    double last_lat;
    double last_lon;
    double last_sog;
    bool in_dark_event = false;
};

#endif //VESSELSTATE_H
