//
// Created by Mark on 3/5/26.
//

#ifndef PORT_H
#define PORT_H

#include <string>
#include <sstream>
struct Port {

    std::string temp;
    std::string wpi_idx;
    std::string region;
    std::string port;
    std::string country;
    std::string lat_str;
    std::string lon_str;

};

inline Port loadPort(std::stringstream& ss) {
    Port p{};
    std::getline(ss, p.wpi_idx, ',');
    std::getline(ss, p.region, ',');
    std::getline(ss, p.port, ',');

    std::getline(ss, p.lat_str, ',');
    std::getline(ss, p.lon_str, ',');
    return p;
}

#endif //PORT_H
