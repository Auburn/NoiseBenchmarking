#include <NoiseBenchmarkInterface.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

class NoiseBenchmark_stb_perlin : public RegisteredNoiseBenchmark<NoiseBenchmark_stb_perlin>
{
public:
    NoiseBenchmark_stb_perlin() : RegisteredNoiseBenchmark( "stb_perlin" ) {}

    bool IsSupported( NoiseType noiseType, size_t dimensionCount ) final
    {
        return noiseType == NoiseType::Perlin;
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        return false;
    }

    bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        int64_t itemIncrement = dimensionSize * dimensionSize * dimensionSize;
        int64_t itemCount = 0;

        for( auto _ : state )
        {
            (void)_;

            for( size_t z = 0; z < dimensionSize; z++ )
            {
                for( size_t y = 0; y < dimensionSize; y++ )
                {
                    for( size_t x = 0; x < dimensionSize; x++ )
                    {
                        benchmark::DoNotOptimize( stb_perlin_noise3( (float)x, (float)y, (float)z, 0, 0, 0 ) );
                    }
                }
            }

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );

        return true;
    }
};