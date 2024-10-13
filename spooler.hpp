#pragma once

#include <thread>
#include <mutex>
#include "job.hpp"
#include "printer.hpp"
#include "logger.hpp"

class Spooler;

class PrinterService {
public:

    explicit PrinterService(Printer &printer) : printer(printer), running(true) {
        logger::print("Printer {}\n", printer.getName());
    }

    std::thread start(Spooler &spooler);

    void stop();

private:
    Printer &printer;
//    std::thread thread;
    bool running;
};


template<typename P>
concept IsPrinter = std::is_same_v<std::decay_t<P>, Printer>;

template<typename... Printers>
concept AllPrinters = (IsPrinter<Printers> && ...);

class Spooler {
public:

    template<AllPrinters... Printers>
    explicit Spooler(Printers &&... prints) : buffer_capacity(10) {
        addPrinter(std::forward<Printers>(prints)...);
    }

    template<AllPrinters... Printers>
    explicit Spooler(unsigned long buffer_size, Printers &&... prints) : buffer_capacity(buffer_size) {
        addPrinter(std::forward<Printers>(prints)...);
    }

    template<IsPrinter P, AllPrinters... Printers>
    void addPrinter(P &&printer, Printers &&... others) {
        printers.push_back(std::forward<P>(printer));
        addPrinter(std::forward<Printers>(others)...);
    }

    void removePrinter(std::string_view printer_name) {
        // Implementar depois
    }

    void start() {
        for (auto &p: printers) {
            consumers.emplace_back([&p, this]() {
                PrinterService service(p);
                service.start(*this).join();
            });
        }
    }

    void stop() {
//        for (auto &t: consumers) {
//            if (t.joinable()) {
//                t.join();
//            }
//        }
        for (auto &printer : printers) {
            printer.printReport();
        }
    }

    void print(pid_t process, std::string_view file, unsigned int num_pages,
               PrintPriority priority = PrintPriority::MEDIUM);

    friend class PrinterService;

private:
    std::priority_queue<PrintRequest, std::vector<PrintRequest>, decltype(&PrintRequest::compare)> buffer;
    unsigned long buffer_capacity;
//    std::vector<PrinterService> services;
    std::vector<Printer> printers;
    std::vector<std::thread> consumers;
    std::mutex mutex;
    std::condition_variable cond_full;
    std::condition_variable cond_empty;
//    float simulation_velocity;

    template<AllPrinters... Args>
    void addPrinter(Args &&... args) {
        (printers.emplace_back(std::forward<Args>(args)), ...);
    }

    void pushJob(const PrintRequest &request);

    PrintRequest popJob();
};
