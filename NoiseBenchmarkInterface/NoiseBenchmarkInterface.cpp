#define DLL_EXPORT
#include "NoiseBenchmarkInterface.h"

std::vector<NoiseBenchmarkInterface*> NoiseBenchmarkInterfaces;

void RegisterNoiseBenchmark( NoiseBenchmarkInterface* p )
{
    NoiseBenchmarkInterfaces.push_back( p );
}

const std::vector<class NoiseBenchmarkInterface*>& GetRegisteredNoiseBenchmarks()
{
    return NoiseBenchmarkInterfaces;
}
