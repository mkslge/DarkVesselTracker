//
// Created by Mark on 3/1/26.
//

#ifndef INGESTIONLAYER_H
#define INGESTIONLAYER_H

#include "../DataSource/PositionUpdate.h"
#include "functional"
#include "queue"
// Simple single-threaded interface
class IngestionLayer {
public:
    using Callback = std::function<void(const PositionUpdate&)>;

    void ingest(PositionUpdate update) {
        // Could add batching or preprocessing here
        callback_(update);
    }

    void setCallback(Callback cb) { callback_ = cb; }

private:
    Callback callback_;
};

#endif //INGESTIONLAYER_H
