#include "spooler.hpp"
#include "logger.hpp"

std::thread PrinterService::start(Spooler &spooler) {
    return std::thread([this, &spooler] {
        while (running) {
            auto job = spooler.popJob();
            printer.printFile(job);
        }
        logger::print("TERMINANDO THREAD {}\n", printer.getName());
    });
}

void PrinterService::stop() {
    logger::print("TERMINANDO THREAD {}\n", printer.getName());
    running = false;
}

void Spooler::print(pid_t process, std::string_view file, unsigned int num_pages, PrintPriority priority) {
    logger::log("Spooler recebendo requisição do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                process, file, static_cast<int>(priority));

    pushJob(PrintRequest(process, file, num_pages, priority));
}

void Spooler::pushJob(const PrintRequest &request) {
    std::unique_lock<std::mutex> lock(mutex);
    cond_full.wait(lock, [this] { return buffer.size() < buffer_capacity; });
    logger::log("Spooler adicionando no buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.getProcess(), request.getFile(), request.getPriorityInt());
    buffer.push(request);
    cond_empty.notify_one();
}

PrintRequest Spooler::popJob() {
    std::unique_lock<std::mutex> lock(mutex);
    cond_empty.wait(lock, [this] { return !buffer.empty(); });
    auto request = buffer.top();
    logger::log("Spooler retirando do buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.getProcess(), request.getFile(), request.getPriorityInt());
    buffer.pop();
    cond_empty.notify_one();
    return request;
}
