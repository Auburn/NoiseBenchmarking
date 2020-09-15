
add_library(Benchstbperlin SHARED
    Benchmark_stb_perlin.cpp)
    
target_link_libraries(Benchstbperlin PRIVATE
    NoiseBenchmarkInterface)

target_include_directories(Benchstbperlin PRIVATE stb)

add_dependencies(NoiseBenchmarking Benchstbperlin)