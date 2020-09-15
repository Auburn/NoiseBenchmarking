#include <memory>
#include <NoiseBenchmarkInterface.h>
#include <FastNoiseSIMD/FastNoiseSIMD.h>

class NoiseBenchmark_FastNoiseSIMD : public RegisteredNoiseBenchmark<NoiseBenchmark_FastNoiseSIMD>
{
public:
    NoiseBenchmark_FastNoiseSIMD() : RegisteredNoiseBenchmark( "FastNoiseSIMD" ) {}

    bool SetupNoise( std::unique_ptr<FastNoiseSIMD>& fastnoise, NoiseType noiseType )
    {
        FastNoiseSIMD::SetSIMDLevel( -1 );

        fastnoise.reset( FastNoiseSIMD::NewFastNoiseSIMD() );

        switch( noiseType )
        {
        case NoiseType::Value: fastnoise->SetNoiseType( FastNoiseSIMD::Value ); break;
        case NoiseType::Perlin: fastnoise->SetNoiseType( FastNoiseSIMD::Perlin ); break;
        case NoiseType::Simplex: fastnoise->SetNoiseType( FastNoiseSIMD::Simplex ); break;
        case NoiseType::Cellular: fastnoise->SetNoiseType( FastNoiseSIMD::Cellular ); break;
        case NoiseType::ValueCubic: fastnoise->SetNoiseType( FastNoiseSIMD::Cubic ); break;
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

        std::unique_ptr<FastNoiseSIMD> fastnoise;
        return SetupNoise( fastnoise, noiseType );
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        return false;
    }

    bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        std::unique_ptr<FastNoiseSIMD> fastnoise;
        if( !SetupNoise( fastnoise, noiseType ) ) return false;

        int64_t itemIncrement = dimensionSize * dimensionSize * dimensionSize;
        int64_t itemCount = 0;

        float* noiseOutput = FastNoiseSIMD::GetEmptySet( itemIncrement );

        for( auto _ : state )
        {
            (void)_;

            fastnoise->FillNoiseSet( noiseOutput, 0, 0, 0, dimensionSize, dimensionSize, dimensionSize );

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );
        state.SetLabel( std::to_string( fastnoise->GetSIMDLevel() ) );
        FastNoiseSIMD::FreeNoiseSet( noiseOutput );

        return true;
    }
};