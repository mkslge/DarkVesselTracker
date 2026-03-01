//
// Created by Mark on 3/1/26.
//

#ifndef CSVDATASOURCE_H
#define CSVDATASOURCE_H



#pragma once
#include "IDataSource.h"
#include <string>

class CSVDataSource : public IDataSource {
public:
    explicit CSVDataSource(const std::string& filepath);

    void start(UpdateCallback callback) override;

private:
    std::string filepath_;
};



#endif //CSVDATASOURCE_H
