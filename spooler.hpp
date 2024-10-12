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

using PrintPriority = PrintJob::Priority;

class Printer {
public:

    explicit Printer(unsigned int id, std::pair<unsigned int, unsigned int> ppm = std::make_pair(60, 30)) :
            id(id), ppm() {}

    void printFile(PrintJob &job) {}

    void printReport() const {
        logger::print("Relatório da impressora {}:\n", id);
        for (auto &p: prints) {
//            logger::print("\tHora da requisição: {}, Hora da impressão: {}, Processo: {}, Arquivo: {}",
//                          p.request_time, p.print_time, p.process, p.file);
        }
    }

    [[nodiscard]] unsigned int getId() const { return id; }

    [[nodiscard]] const std::vector<PrintJob> &getPrints() const { return prints; }

private:
    unsigned int id;
    std::pair<unsigned int, unsigned int> ppm; // ppm: pages per minute<black-write, color>
    std::vector<PrintJob> prints;

    [[nodiscard]] auto calculatePrintLatency(std::string_view file, unsigned long file_size, unsigned int num_pages,
                                             unsigned int dpi) const;
};

template<typename T>
concept IsPrinter = std::is_same_v<std::decay_t<T>, Printer>;

template<typename... Printers>
concept AllPrinters = (IsPrinter<Printers> && ...);

class Spooler {
public:

    template<AllPrinters... Printers>
    explicit Spooler(Printers &&... prints) : buffer_capacity(10) {
        addPrinter(std::forward<Printers>(prints)...);
        printPrinters();
    }

    template<AllPrinters... Printers>
    explicit Spooler(unsigned long buffer_capacity, Printers &&... prints) : buffer_capacity(buffer_capacity) {
        addPrinter(std::forward<Printers>(prints)...);
        printPrinters();
    }

    template<IsPrinter P, AllPrinters... Printers>
    void addPrinter(P &&printer, Printers &&... others) {
        printers.emplace_back(std::forward<P>(printer));
        addPrinter(std::forward<Printers>(others)...);
    }

    void removePrinter(unsigned int id) {}

    void printPrinters() const {
        std::cout << "SPOOLER: " << buffer_capacity << std::endl;
        for (const auto &printer: printers) {
            std::cout << "Printer ID: " << printer.getId() << std::endl;
        }
    }

    void print(pid_t process, std::string_view file, unsigned long file_size,
               PrintPriority priority = PrintPriority::MEDIUM);

private:
    std::priority_queue<PrintJob, std::vector<PrintJob>, decltype(&PrintJob::compare)> buffer;
    std::vector<Printer> printers;
    unsigned long buffer_capacity;
    std::mutex mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;
//    float simulation_velocity;

    template<AllPrinters... Args>
    void addPrinter(Args &&... args) {
        (printers.emplace_back(std::forward<Args>(args)), ...);
    }

    void pushJob(const PrintJob &request);

    PrintJob popJob();
};
