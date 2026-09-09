#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
public:
    Timer() { reset(); }

    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed_milliseconds() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }

    double elapsed_microseconds() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::micro>(end_time - start_time).count();
    }

    double elapsed_seconds() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end_time - start_time).count();
    }
};
