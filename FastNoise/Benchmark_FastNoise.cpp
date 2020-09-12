#include <NoiseBenchmarkInterface.h>
#include <FastNoise.h>

class NoiseBenchmark_FastNoise : public RegisteredNoiseBenchmark<NoiseBenchmark_FastNoise>
{
public:
    NoiseBenchmark_FastNoise() : RegisteredNoiseBenchmark( "FastNoise" ) { }

    bool SetupNoise( benchmark::State& state, FastNoise& fastnoise, NoiseType noiseType )
    {
        fastnoise.SetCellularReturnType( FastNoise::Distance );
        fastnoise.SetCellularDistanceFunction( FastNoise::Euclidean );

        switch( noiseType )
        {
        case NoiseType::Value: fastnoise.SetNoiseType( FastNoise::Value ); break;
        case NoiseType::Perlin: fastnoise.SetNoiseType( FastNoise::Perlin ); break;
        case NoiseType::Simplex: fastnoise.SetNoiseType( FastNoise::Simplex ); break;
        case NoiseType::Cellular: fastnoise.SetNoiseType( FastNoise::Cellular ); break;
        case NoiseType::ValueCubic: fastnoise.SetNoiseType( FastNoise::Cubic ); break;
        default: state.SkipWithError( "NoiseType not supported" ); return false;
        }
        return true;
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        FastNoise fastnoise;
        if( !SetupNoise( state, fastnoise, noiseType ) ) return false;

        int64_t itemIncrement = dimensionSize * dimensionSize;
        int64_t itemCount = 0;

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

        return true;
    }

    bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        FastNoise fastnoise;
        if( !SetupNoise( state, fastnoise, noiseType ) ) return false;

        int64_t itemIncrement = dimensionSize * dimensionSize;
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
                        benchmark::DoNotOptimize( fastnoise.GetNoise( (float)x, (float)y, (float)z ) );
                    }
                }
            }

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );

        return true;
    }
};
