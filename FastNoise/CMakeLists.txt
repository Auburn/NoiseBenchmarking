
add_library(BenchFastNoise SHARED
    Benchmark_FastNoise.cpp
    FastNoise/FastNoise.cpp)
    
target_link_libraries(BenchFastNoise PRIVATE
    benchmark::benchmark)

target_include_directories(BenchFastNoise PRIVATE FastNoise)