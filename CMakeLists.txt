cmake_minimum_required(VERSION 3.30)
project(print-spooler)

set(CMAKE_CXX_STANDARD 23)

add_executable(print-spooler main.cpp
        simulator.cpp
        simulator.hpp
        logger.hpp
        utils.hpp
        spooler.cpp
        spooler.hpp
        printer.cpp
        printer.hpp
        job.hpp)
