//
// Created by Mark on 3/1/26.
//

#ifndef POSITIONUPDATE_H
#define POSITIONUPDATE_H

#pragma once
#include <cstdint>




struct PositionUpdate {
    uint32_t mmsi;          // MMSI fits in 32 bits
    uint64_t timestamp;     // epoch seconds
    double lat;
    double lon;
    double sog;             // speed over ground (knots)
};



#endif //POSITIONUPDATE_H
