#include <iostream>
#include "DataSource/CSVDataSource.h"
#include "IngestionLayer/VesselTracker.h"
#include "IngestionLayer/PortLoader.h"
int main() {
    CSVDataSource source("/Users/markseeliger/Coding/VesselDetect/data/AIS_2024_12_29.csv");
    SpatialIndex spatialIndex;
    PortLoader::loadPortsFromCSV("/Users/markseeliger/Coding/VesselDetect/data/ports.csv", spatialIndex);

    VesselTracker tracker(spatialIndex, 3600);
    source.start([&tracker](const PositionUpdate& update) {
        tracker.updateVessel(update);
        /*std::cout << "Vessel: " << update.mmsi
                  << " @ (" << update.lat << ", "
                  << update.lon << ")"
                  << " speed=" << update.sog
                  << " time=" << update.timestamp
                  << "\n";*/
    });
    tracker.reviewDarkEvents(0);
    return 0;

    return 0;
}