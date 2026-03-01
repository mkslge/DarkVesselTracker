
#include <gtest/gtest.h>
#include "../DataSource/CSVDataSource.h"
#include "../DataSource/PositionUpdate.h"
#include <vector>
#include <string>

TEST(CSVDataSourceTest, ReadsSampleCSV) {
    CSVDataSource source("/Users/markseeliger/Coding/VesselDetect/Test/test_data.csv");

    std::vector<PositionUpdate> updates;

    source.start([&updates](const PositionUpdate& update){
        updates.push_back(update);
    });

    // There should be 2 entries
    ASSERT_EQ(updates.size(), 2);

    // First entry
    EXPECT_EQ(updates[0].mmsi, 123456789);
    EXPECT_DOUBLE_EQ(updates[0].lat, 25.12);
    EXPECT_DOUBLE_EQ(updates[0].lon, 54.22);
    EXPECT_DOUBLE_EQ(updates[0].sog, 12.4);
    // Timestamp check: convert 2026-03-01T12:00:00 manually if needed
    EXPECT_EQ(updates[0].timestamp, 1772366400); // adjust epoch seconds

    // Second entry
    EXPECT_EQ(updates[1].mmsi, 987654321);
    EXPECT_DOUBLE_EQ(updates[1].lat, 26.00);
    EXPECT_DOUBLE_EQ(updates[1].lon, 55.00);
    EXPECT_DOUBLE_EQ(updates[1].sog, 0.0);
}
