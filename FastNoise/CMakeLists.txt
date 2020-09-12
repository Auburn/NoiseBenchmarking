
add_library(BenchFastNoise SHARED
    Benchmark_FastNoise.cpp
    FastNoise/FastNoise.cpp)
    
target_link_libraries(BenchFastNoise PRIVATE
    NoiseBenchmarkInterface)

target_include_directories(BenchFastNoise PRIVATE FastNoise)

add_dependencies(NoiseBenchmarking BenchFastNoise)