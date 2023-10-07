#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
    Timer(const std::string &taskName, bool verbose = false)
            : taskName(taskName)
            , verbose(verbose)
            , start_time(std::chrono::high_resolution_clock::now()) {
    }

    ~Timer() {
        if (verbose) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            std::cout << taskName << ": " << elapsed_time << " milliseconds" << std::endl;
        }
    }

private:
    std::string taskName;
    bool verbose;
    std::chrono::high_resolution_clock::time_point start_time;
};
