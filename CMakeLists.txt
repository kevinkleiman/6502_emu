cmake_minimum_required(VERSION 3.19)
project(6502_emu)

set(CMAKE_CXX_STANDARD 14)

add_executable(6502_emu src/cpu.cpp src/memory.cpp src/cpu.h src/memory.h)