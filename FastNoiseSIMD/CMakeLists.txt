
add_library(BenchFastNoiseSIMD SHARED
    Benchmark_FastNoiseSIMD.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_internal.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp
    FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp
    )
    
add_compile_definitions(FN_COMPILE_AVX512)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(BenchFastNoiseSIMD PRIVATE /GL- /GS- /fp:fast)
    set_source_files_properties(FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp PROPERTIES COMPILE_FLAGS "/arch:AVX2")
    set_source_files_properties(FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp PROPERTIES COMPILE_FLAGS "/arch:AVX512")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
if(MSVC)
    target_compile_options(BenchFastNoiseSIMD PRIVATE /GS- /fp:fast)
endif()
    set_source_files_properties(FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp PROPERTIES COMPILE_FLAGS "-msse4.1")
    set_source_files_properties(FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp PROPERTIES COMPILE_FLAGS "-mavx2 -mfma")
    set_source_files_properties(FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp PROPERTIES COMPILE_FLAGS "-mavx512f -mavx512dq")
endif()


target_link_libraries(BenchFastNoiseSIMD PRIVATE
    NoiseBenchmarkInterface)

target_include_directories(BenchFastNoiseSIMD PRIVATE FastNoiseSIMD)

add_dependencies(NoiseBenchmarking BenchFastNoiseSIMD)