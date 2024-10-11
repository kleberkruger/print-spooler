#pragma once

#include <csignal>
#include <chrono>
#include <expected>
#include "logger.hpp"
#include "utils.hpp"

#include <string_view>
#include <chrono>
#include <expected>

struct PrintJob {

    enum class Priority {
        VERY_HIGH = 1,
        HIGH = 2,
        MEDIUM = 3,
        LOW = 4,
        VERY_LOW = 5
    };

    const pid_t process;
    const std::string_view file;
//    const unsigned long file_size;
    const unsigned int num_pages;
//    const unsigned int dpi; // dots per inch
    const Priority priority;
    const std::chrono::system_clock::time_point request_time;
    std::optional<std::chrono::system_clock::time_point> print_time;

    constexpr PrintJob(pid_t process, std::string_view file, unsigned int num_pages,
                       Priority priority = Priority::MEDIUM,
                       std::chrono::system_clock::time_point request_time = std::chrono::system_clock::now()) :
            process(process), file(file), num_pages(num_pages), priority(priority),
            request_time(request_time) {}

    PrintJob(const PrintJob &) = default;

    [[nodiscard]] auto request_time_str() const {
        return timeToString(request_time);
    }

    [[nodiscard]] auto print_time_str() const {
        return print_time.has_value() ? std::make_optional(timeToString(*print_time)) : std::nullopt;
    }

    static constexpr std::expected<Priority, std::string> priorityOf(int priority) {
        switch (priority) {
            case 1:
                return Priority::VERY_HIGH;
            case 2:
                return Priority::HIGH;
            case 3:
                return Priority::MEDIUM;
            case 4:
                return Priority::LOW;
            case 5:
                return Priority::VERY_LOW;

            default:
                return std::unexpected("Invalid priority value");
        }
    }

    static constexpr bool compare(const PrintJob &a, const PrintJob &b) {
        return (a.priority == b.priority) ? (a.request_time > b.request_time)
                                          : (a.priority > b.priority);
    }
};

//class PrintJob {
//public:
//
//    enum class Priority {
//        VERY_HIGH = 1,
//        HIGH = 2,
//        MEDIUM = 3,
//        LOW = 4,
//        VERY_LOW = 5
//    };
//
//    PrintJob(pid_t process, std::string_view file, unsigned long file_size,
//             Priority priority = Priority::MEDIUM,
//             std::chrono::system_clock::time_point request_time = std::chrono::system_clock::now()) :
//             process(process), file(file), file_size(file_size), priority(priority), request_time(request_time) {}
//
//    PrintJob(const PrintJob &orig) = default;
//
//    [[nodiscard]] auto getProcess() const { return process; }
//
//    [[nodiscard]] const auto &getFile() const { return file; }
//
//    [[nodiscard]] auto getFileSize() const { return file_size; }
//
//    [[nodiscard]] auto getPriority() const { return priority; }
//
//    [[nodiscard]] const auto &getRequestTime() const { return request_time; }
//
//    [[nodiscard]] auto getRequestTimeStr() const { return timeToString(request_time); }
//
//    [[nodiscard]] const auto &getPrintTime() const { return print_time; }
//
//    [[nodiscard]] auto getPrintTimeStr() const { return timeToString(print_time); }
//
//    void setPrintTime(const auto &printTime) { print_time = printTime; }
//
//    static constexpr std::expected<Priority, std::string> priorityOf(int priority) {
//        switch (priority) {
//            case 1: return Priority::VERY_HIGH;
//            case 2: return Priority::HIGH;
//            case 3: return Priority::MEDIUM;
//            case 4: return Priority::LOW;
//            case 5: return Priority::VERY_LOW;
//
//            default: return std::unexpected("Invalid priority value");
//        }
//    }
//
//    static constexpr bool compare(const PrintJob &a, const PrintJob &b) {
//        return (a.priority == b.priority) ? (a.request_time > b.request_time)
//                                          : (a.priority > b.priority);
//    }
//
//private:
//
//    pid_t process;
//    std::string_view file;
//    unsigned long file_size;
//    Priority priority;
//    std::chrono::system_clock::time_point request_time;
//    std::chrono::system_clock::time_point print_time;
//};

using PrintPriority = PrintJob::Priority;

class Printer {
public:
    explicit Printer(unsigned int id, std::pair<unsigned int, unsigned int> ppm = std::make_pair(30, 60)) :
            id(id), ppm() {}

    void printFile(PrintJob &job) {}

    void printReport(PrintJob &request) const {
        logger::print("Relatório da impressora {}:\n", id);
        for (auto &p: prints) {
//            logger::print("\tHora da requisição: {}, Hora da impressão: {}, Processo: {}, Arquivo: {}",
//                          p.request_time, p.print_time, p.process, p.file);
        }
    }

private:
    unsigned int id;
    std::pair<unsigned int, unsigned int> ppm; // ppm: pages per minute<black-write, color>
    std::vector<PrintJob> prints;

    auto calculatePrintLatency(std::string_view file, unsigned long file_size, unsigned int num_pages,
                               unsigned int dpi) const;
};

class Spooler {
public:

//    explicit Spooler(unsigned long buffer_size = 10);

    template<typename... Printers>
    Spooler(unsigned long buffer_size = 10, Printers &... printers) : buffer_capacity(buffer_size) {

    }

    void addPrinter(Printer &printer) {}

    void removePrinter(unsigned int id) {}

    void print(pid_t process, std::string_view file, unsigned long file_size,
               PrintPriority priority = PrintPriority::MEDIUM);

private:
    std::priority_queue<PrintJob, std::vector<PrintJob>, decltype(&PrintJob::compare)> buffer;
    unsigned long buffer_capacity;
    std::mutex mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;

    void pushJob(const PrintJob &request);

    PrintJob popJob();
};
