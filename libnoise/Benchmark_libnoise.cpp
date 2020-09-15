#include <memory>
#include <NoiseBenchmarkInterface.h>
#include <noise.h>

class NoiseBenchmark_libnoise : public RegisteredNoiseBenchmark<NoiseBenchmark_libnoise>
{
public:
    NoiseBenchmark_libnoise() : RegisteredNoiseBenchmark( "libnoise" ) {}

    bool SetupNoise( std::unique_ptr<noise::module::Module>& noise, NoiseType noiseType )
    {
        switch( noiseType )
        {
        case NoiseType::Perlin:
        {
            auto* perlin = new noise::module::Perlin();
            perlin->SetOctaveCount( 1 );
            perlin->SetNoiseQuality( noise::QUALITY_BEST );
            noise.reset( perlin );
            break;
        }
        case NoiseType::Cellular: noise.reset( new noise::module::Voronoi() ); break;
        default: return false;
        }
        return true;
    }

    bool IsSupported( NoiseType noiseType, size_t dimensionCount ) final
    {
        if( dimensionCount != 3 )
        {
            return false;
        }

        std::unique_ptr<noise::module::Module> noise;
        return SetupNoise( noise, noiseType );
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        return false;
    }

    bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        std::unique_ptr<noise::module::Module> noise;
        if( !SetupNoise( noise, noiseType ) ) return false;

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
                        benchmark::DoNotOptimize( noise->GetValue( (double)x, (double)y, (double)z ) );
                    }
                }
            }

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );

        return true;
    }
};