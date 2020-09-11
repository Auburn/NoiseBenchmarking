#include <NoiseBenchmarkInterface.h>
#include <FastNoise.h>

class NoiseBenchmark_FastNoise : public NoiseBenchmarkInterface
{
public:
    NoiseBenchmark_FastNoise() : NoiseBenchmarkInterface( "FastNoise" )
    {
     //   *(int*)0 = 1;
    }

    benchmark::internal::Benchmark* RegisterBenchmark2D( const char* benchName, NoiseType noiseType, size_t dimensionSize ) final
    {
        auto bench = [dimensionSize]( benchmark::State& state, FastNoise fastnoise )
        {
            size_t itemIncrement = dimensionSize * dimensionSize;
            size_t itemCount = 0;

            for( auto _ : state )
            {
                (void)_;

                for( size_t y = 0; y < dimensionSize; y++ )
                {
                    for( size_t x = 0; x < dimensionSize; x++ )
                    {
                        benchmark::DoNotOptimize( fastnoise.GetNoise( (float)x, (float)y ) );
                    }
                }

                itemCount += itemIncrement;
            }

            state.SetItemsProcessed( itemCount );
        };

        FastNoise fastnoise;
        fastnoise.SetCellularReturnType( FastNoise::Distance );
        fastnoise.SetCellularDistanceFunction( FastNoise::Euclidean );

        switch( noiseType )
        {
        case NoiseType::Value: fastnoise.SetNoiseType( FastNoise::Value ); break;
        case NoiseType::Perlin: fastnoise.SetNoiseType( FastNoise::Perlin ); break;
        case NoiseType::Simplex: fastnoise.SetNoiseType( FastNoise::Simplex ); break;
        case NoiseType::Cellular: fastnoise.SetNoiseType( FastNoise::Cellular ); break;
        case NoiseType::Cubic: fastnoise.SetNoiseType( FastNoise::Cubic ); break;
        default: return nullptr;
        }

        return benchmark::RegisterBenchmark( benchName, bench, fastnoise );
    }
};

NoiseBenchmark_FastNoise nbFastNoise = {};