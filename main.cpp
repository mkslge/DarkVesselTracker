#include <iostream>
#include <thread>

#include "DataSource/CSVDataSource.h"
#include "IngestionLayer/VesselTracker.h"
#include "PortLoading/PortLoader.h"
int main() {
    CSVDataSource source("/Users/markseeliger/Coding/VesselDetect/data/AIS_2024_12_29.csv");
    SpatialIndex spatialIndex;
    PortLoader::loadPortsFromCSV("/Users/markseeliger/Coding/VesselDetect/data/port_dataset.csv", spatialIndex);

    VesselTracker tracker(spatialIndex, 3600);

    std::thread monitor(&VesselTracker::monitorLoop, &tracker);
    std::thread dark_event_scanner(&VesselTracker::eventProcessor, &tracker);
    long long vessel_events = 0;
    source.start([&tracker, &vessel_events](const PositionUpdate& update) {
        //std::cout << "Processed " << vessel_events++ << " Vessel events..." << std::endl;
        tracker.updateVessel(update);
    });
    tracker.stop();
    monitor.join();
    dark_event_scanner.join();


    return 0;
}