# CMakeList.txt : CMake project for FastNoise
cmake_minimum_required(VERSION 3.7.1)

project(NoiseBenchmarking)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(FastNoise)
add_subdirectory(FastNoise2)
add_subdirectory(FastNoiseLite)
add_subdirectory(FastNoiseSIMD)
