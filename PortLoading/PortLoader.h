//
// Created by Mark on 3/1/26.
//

#ifndef PORTLOADER_H
#define PORTLOADER_H

#include "../IngestionLayer/SpatialIndex.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Port.h"

namespace PortLoader {
    inline void loadPortsFromCSV(const std::string& filepath, SpatialIndex& index) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open ports file");
        }

        std::string line;
        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Port port = loadPort(ss);

            double lat = std::stod(port.lat_str);
            double lon = std::stod(port.lon_str);

            index.addPort(lat, lon);
        }
    }
}

#endif //PORTLOADER_H
