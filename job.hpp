#pragma once

#include <csignal>
#include <string>
#include <chrono>
#include <expected>
#include "utils.hpp"

class PrintJob {
public:

    [[nodiscard]] pid_t getProcess() const { return process; }

    [[nodiscard]] const std::string &getFile() const { return file; }

    [[nodiscard]] unsigned int getNumPages() const { return num_pages; }

    [[nodiscard]] const std::chrono::system_clock::time_point &getRequestTime() const { return request_time; }

    [[nodiscard]] std::string getRequestTimeStr() const { return timeToString(request_time); }

protected:

    PrintJob(const pid_t process, const std::string_view file, unsigned int num_pages,
             std::chrono::system_clock::time_point request_time = std::chrono::system_clock::now()) :
            process(process), file(file), num_pages(num_pages), request_time(request_time) {}

    pid_t process;
    std::string file;
    unsigned int num_pages;
    std::chrono::system_clock::time_point request_time;
    // should we add attributes for <file_size> and print <dpi>?
};

class PrintRequest : public PrintJob {
public:
    enum class Priority {
        VERY_HIGH = 1,
        HIGH = 2,
        MEDIUM = 3,
        LOW = 4,
        VERY_LOW = 5
    };

    explicit PrintRequest(const pid_t process, const std::string_view &file, unsigned int num_pages, Priority priority,
                          const std::chrono::system_clock::time_point &request_time = std::chrono::system_clock::now()) :
            PrintJob(process, file, num_pages, request_time),
            priority(priority) {}

    PrintRequest(const PrintRequest &orig) = default;

    [[nodiscard]] Priority getPriority() const { return priority; }

    [[nodiscard]] int getPriorityInt() const { return static_cast<int>(priority); }

    static constexpr std::expected<Priority, std::string> priorityOf(int priority) {
        switch (priority) {
            case 1: return Priority::VERY_HIGH;
            case 2: return Priority::HIGH;
            case 3: return Priority::MEDIUM;
            case 4: return Priority::LOW;
            case 5: return Priority::VERY_LOW;

            default: return std::unexpected("Invalid priority value");
        }
    }

    static constexpr bool compare(const PrintRequest &a, const PrintRequest &b) {
        return (a.priority == b.priority) ? (a.request_time > b.request_time)
                                          : (a.priority > b.priority);
    }

private:
    Priority priority;
};

using PrintPriority = PrintRequest::Priority;

class PrintedJob : public PrintJob {
public:

    explicit PrintedJob(const PrintRequest &request,
                        std::chrono::system_clock::time_point print_time = std::chrono::system_clock::now()) :
            PrintJob(request.getProcess(), request.getFile(), request.getNumPages(),
                     request.getRequestTime()),
            print_time(print_time) {}

    PrintedJob(const PrintedJob &orig) = default;

    [[nodiscard]] const std::chrono::system_clock::time_point &getPrintTime() const { return print_time; }

    [[nodiscard]] std::string getPrintTimeStr() const { return timeToString(print_time); }

private:
    std::chrono::system_clock::time_point print_time;
};
