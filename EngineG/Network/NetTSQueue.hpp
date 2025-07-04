#pragma once

#include <deque>
#include <mutex>

namespace EngineG::Network {

template <typename T>
class TSQueue {
public:
    TSQueue() = default;
    TSQueue(const TSQueue&) = delete;
    virtual ~TSQueue() { clear(); }

public:
    const T& front() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }

    const T& back() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    void push_back(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.push_back(item);

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    void push_front(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.push_front(item);

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    bool empty() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }

    size_t size() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    void clear() {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }

    T pop_front() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }

    T pop_back() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }

    void wait() {
        while (empty()) {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }

protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;
};

}  // namespace EngineG::Network