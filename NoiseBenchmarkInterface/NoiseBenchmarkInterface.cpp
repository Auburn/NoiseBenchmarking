#define DLL_EXPORT
#include "NoiseBenchmarkInterface.h"

std::vector<NoiseBenchmarkInterface*> NoiseBenchmarkInterfaces;

void RegisterNoiseBenchmarkInterface( NoiseBenchmarkInterface* p )
{
    NoiseBenchmarkInterfaces.push_back( p );
}

const std::vector<class NoiseBenchmarkInterface*>& GetNoiseBenchmarkInterfaces()
{
    return NoiseBenchmarkInterfaces;
}
