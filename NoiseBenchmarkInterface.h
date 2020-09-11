#pragma once

#include <benchmark/benchmark.h>
#include <magic_enum.h>
#include <vector>
#include <string>

#ifdef DLL_EXPORT
#define DLL_FUNC __declspec(dllexport)
#else
#define DLL_FUNC __declspec(dllimport)
#endif

DLL_FUNC void RegisterNoiseBenchmarkInterface( class NoiseBenchmarkInterface* );

class NoiseBenchmarkInterface
{
public:
    enum class NoiseType
    {
        Value,
        Perlin,
        Simplex,
        OpenSimplex,
        Cellular,
        Cubic,
        EnumMax
    };

    NoiseBenchmarkInterface( const char* name ) : mName( name )
    {
        RegisterNoiseBenchmarkInterface( this );
    }

    std::string FormatBenchmarkName( NoiseType noiseType, size_t dimensionSize )
    {
        std::string benchName = magic_enum::enum_name( noiseType ).data();

        benchName += '_';
        benchName += std::to_string( dimensionSize );
        benchName += "D_";
        benchName += mName;

        return benchName;
    }

    virtual benchmark::internal::Benchmark* RegisterBenchmark2D( const char* benchName, NoiseType noiseType, size_t dimensionSize ) = 0;

protected:
    const char* mName;
};
