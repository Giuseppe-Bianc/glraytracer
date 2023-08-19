#pragma once
#include "headers.h"

class Timer {
public:
#pragma optimize("gt", on)
    Timer() noexcept : start_time(std::chrono::high_resolution_clock::now()) {}
#pragma optimize("gt", on)
    inline void stop() noexcept {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

#pragma optimize("gt", on)
    inline double elapsedNanoseconds() const noexcept {
        const auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration<double, std::nano>(end - start_time).count();
    }

#pragma optimize("gt", on)
    inline double elapsedMicroseconds() const noexcept { return elapsedNanoseconds() / 1000.0; }

#pragma optimize("gt", on)
    inline double elapsedMilliseconds() const noexcept { return elapsedNanoseconds() / 1'000'000.0; }
#pragma optimize("gt", on)
    inline double elapsedSeconds() const noexcept { return elapsedNanoseconds() / 1'000'000'000.0; }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running = true;
};
