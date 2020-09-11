
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "" FORCE)
add_subdirectory(FastNoise2)

add_library(BenchFastNoise2
	Benchmark_FastNoise2.cpp)
	
target_link_libraries(BenchFastNoise2 PRIVATE
    FastNoise)
 
add_dependencies(BenchFastNoise2 FastNoise)