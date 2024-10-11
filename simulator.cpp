#include "simulator.hpp"

std::thread Process::startPrintOrders(Spooler &spooler) const {
    return std::thread([this, &spooler] {
        int n = generateRandomNumber(5, 10);
        logger::print("Processo [{}] enviará {} documentos\n", pid, n);
        for (int i = 0; i < n; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomNumber(0, 5000)));
            spooler.print(pid, getRandomFile(), *PrintRequest::valueOf(generateRandomNumber(1, 5)));
        }
    });
}

void Simulator::start(unsigned int num_process, unsigned int num_printers) {
    Printer printer1(1, 150);
    Printer printer2(2, 150);
    Printer printer3(3, 200);

//    Spooler spooler(printer1, printer2, printer3);
//    std::vector<std::pair<Process, std::thread>> process;
//
//    for (int pid = 1; pid <= num_process; pid++) {
//        Process p(pid);
//        process.emplace_back(p, p.startPrintOrders(spooler));
//    }
//
//    for (auto &t: process) {
//        if (t.second.joinable()) {
//            t.second.join();
//        }
//    }

    logger::log("Todas as threads de impressão terminaram.\n");
}
