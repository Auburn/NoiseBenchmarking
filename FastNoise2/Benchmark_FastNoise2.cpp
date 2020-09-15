#include <NoiseBenchmarkInterface.h>
#include <FastNoise/FastNoise.h>

template<FastSIMD::eLevel LEVEL>
class NoiseBenchmark_FastNoise2 : public RegisteredNoiseBenchmark<NoiseBenchmark_FastNoise2<LEVEL>>
{
public:
    NoiseBenchmark_FastNoise2() : RegisteredNoiseBenchmark<NoiseBenchmark_FastNoise2<LEVEL>>( "FastNoise2" ) {}

    using NoiseType = NoiseBenchmarkInterface::NoiseType;

    bool SetupNoise( FastNoise::SmartNode<>& fastnoise, NoiseType noiseType )
    {
        FastSIMD::eLevel testSIMD = LEVEL;

        switch( noiseType )
        {
        case NoiseType::Value: fastnoise = FastNoise::New<FastNoise::Value>( testSIMD ); break;
        case NoiseType::Perlin: fastnoise = FastNoise::New<FastNoise::Perlin>( testSIMD ); break;
        case NoiseType::Simplex: fastnoise = FastNoise::New<FastNoise::Simplex>( testSIMD ); break;
        case NoiseType::OpenSimplex2: fastnoise = FastNoise::New<FastNoise::OpenSimplex2>( testSIMD ); break;
        //case NoiseType::OpenSimplex2S: FastNoise::New<FastNoise::Value>(testSIMD) break;
        case NoiseType::Cellular: fastnoise = FastNoise::New<FastNoise::CellularDistance>( testSIMD ); break;
        //case NoiseType::ValueCubic: FastNoise::New<FastNoise::Value>(testSIMD) break;
        default: return false;
        }
        return true;
    }

    bool IsSupported( NoiseType noiseType, size_t dimensionCount ) final
    {
        FastNoise::SmartNode<> fastnoise;
        return SetupNoise( fastnoise, noiseType );
    }

    bool Benchmark2D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        FastNoise::SmartNode<> fastnoise;
        if( !SetupNoise( fastnoise, noiseType ) ) return false;

        int64_t itemIncrement = dimensionSize * dimensionSize;
        int64_t itemCount = 0;

        std::vector<float> noiseOutput( itemIncrement );

        for( auto _ : state )
        {
            (void)_;

            fastnoise->GenUniformGrid2D( noiseOutput.data(), 0, 0, dimensionSize, dimensionSize, 0.02f, 1337 );

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );
        state.SetLabel( magic_enum::flags::enum_name( fastnoise->GetSIMDLevel() ) );

        return true;
    }

    bool Benchmark3D( benchmark::State& state, NoiseType noiseType, size_t dimensionSize ) final
    {
        FastNoise::SmartNode<> fastnoise;
        if( !SetupNoise( fastnoise, noiseType ) ) return false;

        int64_t itemIncrement = dimensionSize * dimensionSize * dimensionSize;
        int64_t itemCount = 0;

        std::vector<float> noiseOutput( itemIncrement );

        for( auto _ : state )
        {
            (void)_;

            fastnoise->GenUniformGrid3D( noiseOutput.data(), 0, 0, 0, dimensionSize, dimensionSize, dimensionSize, 0.02f, 1337 );

            itemCount += itemIncrement;
        }

        state.SetItemsProcessed( itemCount );
        state.SetLabel( magic_enum::flags::enum_name( fastnoise->GetSIMDLevel() ) );

        return true;
    }
};

//template class NoiseBenchmark_FastNoise2<FastSIMD::Level_SSE41>;
template class NoiseBenchmark_FastNoise2<FastSIMD::Level_AVX2>;
//template class NoiseBenchmark_FastNoise2<FastSIMD::Level_AVX512>;
