#pragma once

#include <string>
#include <vector>
#include "job.hpp"

class Printer {
public:

    explicit Printer(std::string_view name,
                     std::pair<unsigned int, unsigned int> ppm = std::make_pair(60, 30));

    virtual ~Printer();

    void printFile(PrintRequest &request);

    void printReport() const;

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] unsigned int getBlackWhitePPM() const { return ppm.first; }

    [[nodiscard]] unsigned int getColorPPM() const { return ppm.second; }

    [[nodiscard]] const std::vector<PrintedJob> &getLog() const { return log; }

private:
    std::string name;
    std::vector<PrintedJob> log;
    std::pair<unsigned int, unsigned int> ppm; // ppm: pages per minute<black-write, color>

    [[nodiscard]] auto calculatePrintLatency(std::string_view file, unsigned long file_size, unsigned int num_pages,
                                             unsigned int dpi) const;
};
