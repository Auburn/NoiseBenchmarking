
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "" FORCE)
add_subdirectory(FastNoise2)

add_library(BenchFastNoise2 SHARED
	Benchmark_FastNoise2.cpp)
	
target_link_libraries(BenchFastNoise2 PRIVATE
	NoiseBenchmarkInterface
    FastNoise)
 
add_dependencies(BenchFastNoise2 FastNoise)

add_dependencies(NoiseBenchmarking BenchFastNoise2)