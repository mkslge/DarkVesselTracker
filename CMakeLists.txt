cmake_minimum_required(VERSION 3.31)
project(VesselDetect)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# ---------------------------
# GoogleTest
# ---------------------------
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()



add_library(DataSource DataSource/CSVDataSource.cpp
        DataSource/CSVDataSource.h
        DataSource/IDataSource.h
        DataSource/PositionUpdate.h
        IngestionLayer/VesselTracker.cpp
        IngestionLayer/VesselTracker.h
)

add_executable(VesselDetect main.cpp
        IngestionLayer/VesselState.h
        IngestionLayer/IngestionLayer.h
        IngestionLayer/SpatialIndex.h
        IngestionLayer/PortLoader.h
        IngestionLayer/Port.h
)

add_executable(testDataSource
        Test/DataSourceTest.cpp
)


target_link_libraries(VesselDetect PRIVATE DataSource)

target_link_libraries(testDataSource PRIVATE DataSource GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(testDataSource)