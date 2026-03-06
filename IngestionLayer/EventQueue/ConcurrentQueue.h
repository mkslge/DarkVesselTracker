//
// Created by Mark on 3/5/26.
//

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <mutex>
#include <queue>
template <typename T>
class ConcurrentQueue {


public:
    void push(const T& in) {
        std::lock_guard<std::mutex> lg(m);
        q.push(in);

    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lg(m);
        if (q.empty()) {
            return false;
        }
        out = q.front();
        q.pop();
        return true;
    }


private:
    std::queue<T> q;
    std::mutex m;
};

#endif //CONCURRENTQUEUE_H

