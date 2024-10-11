#include "spooler.hpp"

void Spooler::print(pid_t process, std::string_view file, PrintPriority priority) {
    logger::log("Spooler recebendo requisição do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                process, file, static_cast<int>(priority));
    pushJob(PrintRequest(process, file, priority));
}

void Spooler::pushJob(const PrintRequest &request) {
    std::unique_lock<std::mutex> lock(mutex);
    cond_full.wait(lock, [this] { return buffer.size() < buffer_capacity; });
    logger::log("Spooler adicionando no buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.process, request.file, static_cast<int>(request.priority));
    buffer.push(request);
    cond_empty.notify_one();
}

PrintRequest Spooler::popJob() {
    std::unique_lock<std::mutex> lock(mutex);
    cond_empty.wait(lock, [this] { return !buffer.empty(); });
    auto request = buffer.top();
    logger::log("Spooler retirando do buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.process, request.file, static_cast<int>(request.priority));
    buffer.pop();
    cond_empty.notify_one();
    return request;
}

