
add_library(NoiseBenchmarkInterface SHARED
    NoiseBenchmarkInterface.cpp)
    
target_link_libraries(NoiseBenchmarkInterface PUBLIC
    benchmark::benchmark)

target_include_directories(NoiseBenchmarkInterface PUBLIC .)