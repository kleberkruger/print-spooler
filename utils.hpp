#pragma once

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random>

inline std::string timeToString(std::chrono::system_clock::time_point time_point,
                                std::string format = "%Y-%m-%d %H:%M:%S") {
    auto show_milliseconds = false;
    if (auto pos = format.find("%s"); pos != std::string::npos) {
        format.erase(pos, 2);
        show_milliseconds = true;
    }

    auto time_t = std::chrono::system_clock::to_time_t(time_point);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), format.c_str());
    if (show_milliseconds) {
        auto duration = time_point.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
        ss << std::setfill('0') << std::setw(3) << milliseconds.count();
    }
    return ss.str();
}

inline int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    return std::uniform_int_distribution<>(min, max)(gen);
}
