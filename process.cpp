#include "process.hpp"

std::thread Process::startPrintOrders(Spooler &spooler) const {
    return std::thread([this, &spooler] {
        int n = generateRandomNumber(5, 10);
        logger::print("Processo [{}] enviará {} documentos\n", pid, n);
        for (int i = 0; i < n; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomNumber(0, 5000)));
            auto file = getRandomFile();
            spooler.print(pid, file.first, file.second,
                          *PrintJob::priorityOf(generateRandomNumber(1, 5)));
        }
    });
}
