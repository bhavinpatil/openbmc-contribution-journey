cmake_minimum_required(VERSION 3.10)
project(ModularCMake)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(modular ${SOURCES})
