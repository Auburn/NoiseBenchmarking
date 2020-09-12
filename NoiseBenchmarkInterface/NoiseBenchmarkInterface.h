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

DLL_FUNC void RegisterNoiseBenchmarkInterface( class NoiseBenchmarkInterface* );
DLL_FUNC const std::vector<class NoiseBenchmarkInterface*>& GetNoiseBenchmarkInterfaces();

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

    std::string FormatBenchmarkName( NoiseType noiseType, size_t dimensionCount )
    {
        std::string benchName = magic_enum::enum_name( noiseType ).data();

        benchName += '_';
        benchName += std::to_string( dimensionCount );
        benchName += "D_";
        benchName += mName;

        return benchName;
    }

    virtual bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) = 0;
    virtual bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) = 0;

protected:
    NoiseBenchmarkInterface( const char* name ) : mName( name ) { }
    const char* mName;
};

template<typename CLASS>
class RegisteredNoiseBenchmarkInterface : public NoiseBenchmarkInterface
{
public:
    RegisteredNoiseBenchmarkInterface( const char* name ) : NoiseBenchmarkInterface( name )
    {
        RegisterNoiseBenchmarkInterface( this );
    }

    static CLASS Instance;
};

template<typename CLASS> CLASS RegisteredNoiseBenchmarkInterface<CLASS>::Instance = {};
