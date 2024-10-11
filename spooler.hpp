#pragma once

#include <csignal>
#include <expected>
#include "logger.hpp"
#include "utils.hpp"

struct PrintRequest {

    enum class Priority {
        VERY_HIGH = 1,
        HIGH = 2,
        MEDIUM = 3,
        LOW = 4,
        VERY_LOW = 5
    };

    static std::expected<Priority, std::string> valueOf(int priority) {
        switch (priority) {
            case 1: return Priority::VERY_HIGH;
            case 2: return Priority::HIGH;
            case 3: return Priority::MEDIUM;
            case 4: return Priority::LOW;
            case 5: return Priority::VERY_LOW;

            default: return std::unexpected("Invalid priority value");
        }
    }

    pid_t process;
    std::string_view file;
    Priority priority;
    std::chrono::system_clock::time_point request_time;
    std::chrono::system_clock::time_point print_time;

    PrintRequest(pid_t process, std::string_view file, Priority priority = Priority::MEDIUM,
                 std::chrono::system_clock::time_point request_time = std::chrono::system_clock::now()) :
            process(process), file(file), priority(priority), request_time(request_time) {}

    PrintRequest(const PrintRequest &orig) = default;

    static bool compare(const PrintRequest &a, const PrintRequest &b) {
        return (a.priority == b.priority) ? (a.request_time > b.request_time)
                                          : (a.priority > b.priority);
    }
};

class PrintReport {

};

using PrintPriority = PrintRequest::Priority;

class Printer {
public:
    explicit Printer(unsigned int id, unsigned int latency = 100) : id(id), latency(latency) {}

    void printFile(PrintRequest &request) {}

    void printReport(PrintRequest &request) const {
        logger::print("Relatório da impressora {}:\n", id);
        for (auto &p: prints) {
            logger::print("\tHora da requisição: {}, Hora da impressão: {}, Processo: {}, Arquivo: {}",
                          p.request_time, p.print_time, p.process, p.file);
        }
    }


private:
    unsigned int id;
    unsigned int latency;
    std::vector<PrintRequest> prints;
};

class Spooler {
public:

//    explicit Spooler(unsigned long buffer_size = 10);

    template<typename... Printers>
    Spooler(unsigned long buffer_size = 10, Printers &... printers) : buffer_capacity(buffer_size) {

    }

    void addPrinter(Printer &printer) {}

    void removePrinter(unsigned int id) {}

    void print(pid_t process, std::string_view file, PrintPriority priority = PrintPriority::MEDIUM);

private:
    std::priority_queue<PrintRequest, std::vector<PrintRequest>, decltype(&PrintRequest::compare)> buffer;
    unsigned long buffer_capacity;
    std::mutex mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;

    void pushJob(const PrintRequest &request);

    PrintRequest popJob();
};
