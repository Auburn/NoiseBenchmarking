
set(BUILD_LIBNOISE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_LIBNOISE_UTILS OFF CACHE BOOL "" FORCE)
add_subdirectory(libnoise)

add_library(Benchlibnoise SHARED
    Benchmark_libnoise.cpp)
    
target_link_libraries(Benchlibnoise PRIVATE
    NoiseBenchmarkInterface
    noise-static)

    
target_include_directories(Benchlibnoise PRIVATE libnoise/src/noise)
 
add_dependencies(Benchlibnoise noise-static)

add_dependencies(NoiseBenchmarking Benchlibnoise)