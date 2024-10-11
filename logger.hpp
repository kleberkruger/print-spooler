#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>
#include "utils.hpp"

class Logger {
public:
    enum class LogLevel {
        DEBUG, INFO, WARNING, ERROR
    };

    static Logger &get() {
        static Logger instance(std::cout, "%H:%M:%S.%s");
        return instance;
    }

    explicit Logger(std::ostream &output = std::cout, std::string_view date_time_format = "%Y-%m-%d %H:%M:%S") :
            output(output), date_time_format(date_time_format) {}

    template<typename... Args>
    void print(const std::string_view format, Args &&... args) const {
        std::lock_guard<std::mutex> lock(mutex);
        output << std::vformat(format, std::make_format_args(args...));
    }

    template<typename... Args>
    void log(const std::string_view format, Args &&... args) const {
        print("{} | {}\n", timeToString(std::chrono::system_clock::now(), date_time_format),
              std::vformat(format, std::make_format_args(args...)));
    }

    template<typename... Args>
    void log(const LogLevel level, const std::string_view format, Args &&... args) const {
        print("[{}] {} | {}\n", levelToString(level), timeToString(std::chrono::system_clock::now(), date_time_format),
              std::vformat(format, std::make_format_args(args...)));
    }

    template<typename... Args>
    void debug(const std::string_view format, Args &&... args) const {
        log(LogLevel::DEBUG, format, args...);
    }

    template<typename... Args>
    void info(const std::string_view format, Args &&... args) const {
        log(LogLevel::INFO, format, args...);
    }

    template<typename... Args>
    void warn(const std::string_view format, Args &&... args) const {
        log(LogLevel::WARNING, format, args...);
    }

    template<typename... Args>
    void error(const std::string_view format, Args &&... args) const {
        log(LogLevel::ERROR, format, args...);
    }

private:
    std::ostream &output;
    mutable std::mutex mutex;
    std::string date_time_format;

    static std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
        }
        return "UNKNOWN";
    }
};

namespace logger {

    template<typename... Args>
    inline void print(const std::string_view format, Args &&... args) {
        Logger::get().print(format, args...);
    }

    template<typename... Args>
    inline void log(const std::string_view format, Args &&... args) {
        Logger::get().log(format, args...);
    }
}

