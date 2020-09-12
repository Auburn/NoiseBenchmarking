# CMakeList.txt : CMake project for FastNoise
cmake_minimum_required(VERSION 3.7.1)

project(NoiseBenchmarking)
set(CMAKE_CXX_STANDARD 17)

#setup build folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/)
#for multi-config builds (e.g. msvc)
if(CMAKE_CONFIGURATION_TYPES)
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PROJECT_BINARY_DIR}/bin/${OUTPUTCONFIG}/)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PROJECT_BINARY_DIR}/lib/${OUTPUTCONFIG}/)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${PROJECT_BINARY_DIR}/lib/${OUTPUTCONFIG}/)
    endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
else()
    #make sure there is a build type
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
    endif()
endif()

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(benchmark)

add_subdirectory(NoiseBenchmarkInterface)
include_directories(NoiseBenchmarkInterface)

add_subdirectory(FastNoise)
# add_subdirectory(FastNoise2)
# add_subdirectory(FastNoiseLite)
# add_subdirectory(FastNoiseSIMD)

add_executable(NoiseBenchmarking 
    "main.cpp")
    
target_link_libraries(NoiseBenchmarking
    benchmark::benchmark
    NoiseBenchmarkInterface
)

add_dependencies(NoiseBenchmarking
    BenchFastNoise)