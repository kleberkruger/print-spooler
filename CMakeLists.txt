cmake_minimum_required(VERSION 3.30)
project(print-spooler)

set(CMAKE_CXX_STANDARD 23)

add_executable(print-spooler main.cpp
        spooler.cpp
        spooler.hpp
        logger.hpp
        utils.hpp
        process.cpp
        process.hpp)
