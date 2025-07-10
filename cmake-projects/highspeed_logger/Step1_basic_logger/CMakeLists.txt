cmake_minimum_required(VERSION 3.10)        # Minimum CMake version required
project(Step1Logger)                        # Name of your project

set(CMAKE_CXX_STANDARD 17)                  # Use C++17 standard
add_executable(logger main.cpp)             # Build 'logger' from main.cpp