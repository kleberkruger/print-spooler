#pragma once

#include <string>
#include <expected>
#include <optional>
#include <thread>

class Spooler;

class Process {
public:
    explicit Process(pid_t pid) : pid(pid) {}

    std::thread start(Spooler &spooler);

    void stop();

    [[nodiscard]] pid_t getPid() const { return pid; }

//    [[nodiscard]] std::thread &getThread() const { return thread; }

private:
    pid_t pid;
//    mutable std::thread thread;

    static std::pair<std::string_view, unsigned int> getRandomFile();
};

class Simulator {
public:

    static void start(int argc, char *argv[]);

    static void start(unsigned int num_process, unsigned int num_printers);

private:
    static constexpr unsigned int DEFAULT_NUMBER_PROCESS = 4;
    static constexpr unsigned int DEFAULT_NUMBER_PRINTERS = 2;

    static std::expected<unsigned int, std::string>
    parseArg(std::string_view arg_name, std::optional<std::string_view> arg_value, int default_value);

    static std::expected<std::pair<unsigned int, unsigned int>, std::string>
    getArgs(int argc, char *argv[]);
};
