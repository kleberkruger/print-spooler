#include "printer.hpp"
#include "logger.hpp"

Printer::Printer(std::string_view name, std::pair<unsigned int, unsigned int> ppm) :
        name(name), ppm({ppm.first, ppm.second}) {}

Printer::~Printer() {
    printReport();
}

void Printer::printFile(PrintRequest &request) {
    logger::log("{} imprimindo arquivo {} do processo {}", name, request.getFile(), request.getProcess());
    log.emplace_back(request);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Printer::printReport() const {
    logger::print(std::format("Relatório da impressora {} <<<\n", name));
    for (auto &job: log) {
        logger::print("Hora da requisição: {}, Hora da impressão: {}, Processo: {}, Arquivo: {}\n",
                      job.getRequestTimeStr().c_str(), job.getPrintTimeStr().c_str(),
                      job.getProcess(), job.getFile());
    }
}

auto Printer::calculatePrintLatency(const std::string_view file, const unsigned long file_size,
                                    const unsigned int num_pages, const unsigned int dpi) const {

    return std::chrono::milliseconds(ppm.first);
}
