#include "spooler.hpp"

void Spooler::print(pid_t process, std::string_view file, unsigned long file_size, PrintPriority priority) {
    logger::log("Spooler recebendo requisição do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                process, file, static_cast<int>(priority));
    pushJob(PrintJob(process, file, file_size, priority));
}

void Spooler::pushJob(const PrintJob &request) {
    std::unique_lock<std::mutex> lock(mutex);
    cond_full.wait(lock, [this] { return buffer.size() < buffer_capacity; });
    logger::log("Spooler adicionando no buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.process, request.file, static_cast<int>(request.priority));
//    buffer.push(request);
    cond_empty.notify_one();
}

PrintJob Spooler::popJob() {
    std::unique_lock<std::mutex> lock(mutex);
    cond_empty.wait(lock, [this] { return !buffer.empty(); });
    auto request = buffer.top();
    logger::log("Spooler retirando do buffer impressão do processo [{}]: {{ arquivo: '{}', prioridade: {} }}",
                request.process, request.file, static_cast<int>(request.priority));
//    buffer.pop();
    cond_empty.notify_one();
    return request;
}

auto Printer::calculatePrintLatency(std::string_view file, unsigned long file_size, unsigned int num_pages,
                                    unsigned int dpi) const {

    static const std::vector<std::string> image_ext = {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg", ".ico"};

    auto pos = file.find_last_of('.');
    auto extension = pos != std::string::npos && pos != 0 ? file.substr(pos) : "";
    if (std::find(image_ext.begin(), image_ext.end(), extension) != image_ext.end()) {
        return std::chrono::milliseconds(ppm.second * generateRandomNumber(1000, 5000));
    }

    return std::chrono::milliseconds(ppm.first * num_pages);
}
