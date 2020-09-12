#include <NoiseBenchmarkInterface.h>
#include <FastNoiseLite.h>

class NoiseBenchmark_FastNoiseLite : public RegisteredNoiseBenchmark<NoiseBenchmark_FastNoiseLite>
{
public:
    NoiseBenchmark_FastNoiseLite() : RegisteredNoiseBenchmark( "FastNoiseLite" ) { }

    bool SetupNoise( benchmark::State& state, FastNoiseLite& fastnoise, NoiseType noiseType )
    {
        fastnoise.SetCellularReturnType( FastNoiseLite::CellularReturnType_Distance );
        fastnoise.SetCellularDistanceFunction( FastNoiseLite::CellularDistanceFunction_Euclidean );

        switch( noiseType )
        {
        case NoiseType::Value: fastnoise.SetNoiseType( FastNoiseLite::NoiseType_Value ); break;
        case NoiseType::Perlin: fastnoise.SetNoiseType( FastNoiseLite::NoiseType_Perlin ); break;
        case NoiseType::OpenSimplex2: fastnoise.SetNoiseType( FastNoiseLite::NoiseType_OpenSimplex2 ); break;
        case NoiseType::Cellular: fastnoise.SetNoiseType( FastNoiseLite::NoiseType_Cellular ); break;
        case NoiseType::ValueCubic: fastnoise.SetNoiseType( FastNoiseLite::NoiseType_ValueCubic ); break;
        default: state.SkipWithError( "NoiseType not supported" ); return false;
        }
        return true;
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        FastNoiseLite fastnoise;
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
        FastNoiseLite fastnoise;
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