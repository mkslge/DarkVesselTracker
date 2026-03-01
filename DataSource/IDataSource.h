//
// Created by Mark on 3/1/26.
//

#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#pragma once
#include <functional>
#include "PositionUpdate.h"

class IDataSource {
public:
    using UpdateCallback = std::function<void(const PositionUpdate&)>;

    virtual ~IDataSource() = default;

    virtual void start(UpdateCallback callback) = 0;
};
#endif //IDATASOURCE_H
