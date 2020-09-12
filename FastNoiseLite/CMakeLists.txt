
add_library(BenchFastNoiseLite SHARED
    Benchmark_FastNoiseLite.cpp)
    
target_link_libraries(BenchFastNoiseLite PRIVATE
    NoiseBenchmarkInterface)

target_include_directories(BenchFastNoiseLite PRIVATE FastNoiseLite/Cpp)

add_dependencies(NoiseBenchmarking BenchFastNoiseLite)