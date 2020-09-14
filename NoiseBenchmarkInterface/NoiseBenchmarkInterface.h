#pragma once

#include <benchmark/benchmark.h>
#include "magic_enum.h"
#include <string>
#include <vector>

#ifdef DLL_EXPORT
#define DLL_FUNC __declspec(dllexport)
#else
#define DLL_FUNC __declspec(dllimport)
#endif

class NoiseBenchmarkInterface
{
public:
    enum class NoiseType
    {
        Value,
        Perlin,
        Simplex,
        OpenSimplex2,
        OpenSimplex2S,
        Cellular,
        ValueCubic,
        EnumMax
    };

    std::string FormatBenchmarkName( NoiseType noiseType, size_t dimensionCount )
    {
        std::string benchName;

        benchName += std::to_string( dimensionCount );
        benchName += "D_";
        benchName += magic_enum::enum_name( noiseType ).data();
        benchName += '_';
        benchName += mName;

        return benchName;
    }

    virtual bool IsSupported( NoiseType noiseType, size_t dimensionCount ) = 0;

    virtual bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) = 0;
    virtual bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) = 0;

protected:
    NoiseBenchmarkInterface( const char* name ) : mName( name ) { }
    const char* mName;
};

DLL_FUNC void RegisterNoiseBenchmark( NoiseBenchmarkInterface* );
DLL_FUNC const std::vector<NoiseBenchmarkInterface*>& GetRegisteredNoiseBenchmarks();

template<typename CLASS>
class RegisteredNoiseBenchmark : public NoiseBenchmarkInterface
{
public:
    RegisteredNoiseBenchmark( const char* name ) : NoiseBenchmarkInterface( name )
    {
        RegisterNoiseBenchmark( &Instance );
    }

    static CLASS Instance;
};

template<typename CLASS> CLASS RegisteredNoiseBenchmark<CLASS>::Instance = {};
