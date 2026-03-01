#include "CSVDataSource.h"
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>
static uint64_t parseTimestamp(const std::string& dt) {
    std::tm tm = {};
    std::istringstream ss(dt);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
#if defined(_WIN32)
    // Windows doesn't have timegm, use _mkgmtime
    return static_cast<uint64_t>(_mkgmtime(&tm));
#else
    return static_cast<uint64_t>(timegm(&tm));
#endif
}

CSVDataSource::CSVDataSource(const std::string& filepath)
    : filepath_(filepath) {}

void CSVDataSource::start(UpdateCallback callback) {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        size_t pos = 0;
        size_t next;

        PositionUpdate update{};

        // MMSI
        next = line.find(',', pos);
        update.mmsi = static_cast<uint32_t>(
            std::stoul(line.substr(pos, next - pos))
        );
        pos = next + 1;

        // BaseDateTime
        next = line.find(',', pos);
        std::string datetime = line.substr(pos, next - pos);
        update.timestamp = parseTimestamp(datetime);
        pos = next + 1;

        // LAT
        next = line.find(',', pos);
        update.lat = std::stod(line.substr(pos, next - pos));
        pos = next + 1;

        // LON
        next = line.find(',', pos);
        update.lon = std::stod(line.substr(pos, next - pos));
        pos = next + 1;

        // SOG
        next = line.find(',', pos);
        update.sog = std::stod(line.substr(pos, next - pos));

        if (update.lat < -90 || update.lat > 90) continue;
        if (update.lon < -180 || update.lon > 180) continue;
        if (update.sog < 0 || update.sog > 60) continue; // sanity

        callback(update);
    }
}