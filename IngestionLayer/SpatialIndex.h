//
// Created by Mark on 3/1/26.
//

//
// SpatialIndex.h
//

#ifndef SPATIALINDEX_H
#define SPATIALINDEX_H

#pragma once
#include <vector>
#include <utility>
#include <cmath>

class SpatialIndex {
public:
    SpatialIndex() = default;

    // Add a known port or anchorage location
    void addPort(double lat, double lon) {
        ports_.emplace_back(lat, lon);
    }

    // Check if a position is near any port within radius_meters
    bool isNearPort(double lat, double lon, double radius_meters) const {
        for (const auto& port : ports_) {
            double distance = haversine(lat, lon, port.first, port.second);
            if (distance <= radius_meters) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<std::pair<double,double>> ports_;

    // Approximate Haversine formula to compute distance in meters
    static double haversine(double lat1, double lon1, double lat2, double lon2) {
        constexpr double R = 6371000.0; // Earth radius in meters
        double dLat = deg2rad(lat2 - lat1);
        double dLon = deg2rad(lon2 - lon1);
        double a = std::sin(dLat/2) * std::sin(dLat/2) +
                   std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
                   std::sin(dLon/2) * std::sin(dLon/2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
        return R * c;
    }

    static double deg2rad(double deg) {
        return deg * M_PI / 180.0;
    }
};

#endif // SPATIALINDEX_H