//
// Created by Mark on 3/1/26.
//

#ifndef PORTLOADER_H
#define PORTLOADER_H

#include "SpatialIndex.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
namespace PortLoader {
    void loadPortsFromCSV(const std::string& filepath, SpatialIndex& index) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open ports file");
        }

        std::string line;
        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, latStr, lonStr, radiusStr;
            std::getline(ss, name, ',');
            std::getline(ss, latStr, ',');
            std::getline(ss, lonStr, ',');
            std::getline(ss, radiusStr, ',');

            double lat = std::stod(latStr);
            double lon = std::stod(lonStr);
            double radius = std::stod(radiusStr); // optional: per-port radius

            index.addPort(lat, lon); // you can also store radius if you extend SpatialIndex
        }
    }
}

#endif //PORTLOADER_H
