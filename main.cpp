#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#elif defined(__linux__)
#include <link.h>
#include <dlfcn.h>
#endif

#include <NoiseBenchmarkInterface.h>

void LoadDynamicLibs()
{
    fs::path directory( "." );

    std::string libExtension;
    std::string libPrefix = "Bench";

#if defined(_WIN32) || defined(_WIN64)
    libExtension = ".dll";
#elif defined(__APPLE__)
    libExtension = ".dylib";
#elif defined( __linux__)
    libExtension = ".so";
#else
    static_assert(false);
#endif
    fs::directory_iterator iter( directory );
    fs::directory_iterator endIter;

    while( iter != endIter )
    {
        fs::path filePath = iter->path();
        std::string fileName = filePath.stem().string();
        std::string extension = filePath.extension().string();

        if( fs::is_regular_file( *iter ) && extension == libExtension && fileName.compare( 0, libPrefix.size(), libPrefix.c_str() ) == 0 )
        {

#if defined(_WIN32) || defined(_WIN64)
            HINSTANCE instance = NULL;

            try
            {
                instance = LoadLibrary( filePath.string().c_str() );
            } //Plugins should autoregister
            catch( ... )
            {
                instance = NULL;
            }

            if( instance == NULL )
            {
                LPVOID lpMsgBuf;
                DWORD error = GetLastError();

                FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    error,
                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                    (LPTSTR)&lpMsgBuf,
                    0,
                    NULL
                );

                std::string message( (char*)lpMsgBuf );
                std::cout << "Failed to load " << fileName << " error: " << message;
                LocalFree( lpMsgBuf );
            }
#elif defined(__APPLE__) || defined( __linux__)
            void* handle;

            dlerror();
            try
            {
                handle = dlopen( filePath.string().c_str(), RTLD_NOW );
            }
            catch( std::exception& except )
            {
                handle = NULL;
            }
            catch( ... )
            {
                handle = NULL;
            }

            if( handle == NULL )
            {
                char* error = dlerror();

                std::cout << "Failed to load " << fileName << " error: " << error;
            }
#else
            static_assert(false);
#endif
        }

        ++iter;
    }
}

int main( int argc, char** argv )
{
    benchmark::Initialize( &argc, argv );
    if( benchmark::ReportUnrecognizedArguments( argc, argv ) ) { return 1; }

    LoadDynamicLibs();

    int64_t dimensionSize2D = 512;
    int64_t dimensionSize3D = 64;

    for( auto noiseType = (NoiseBenchmarkInterface::NoiseType)0;
        noiseType < NoiseBenchmarkInterface::NoiseType::EnumMax;
        noiseType = (NoiseBenchmarkInterface::NoiseType)((int)noiseType + 1) )
    {
        for( size_t dimensionCount = 2; dimensionCount <= 3; dimensionCount++ )
        {
            for( NoiseBenchmarkInterface* noiseBenchmarkInterface : GetNoiseBenchmarkInterfaces() )
            {
                std::string benchName = noiseBenchmarkInterface->FormatBenchmarkName( noiseType, dimensionCount );

                if( dimensionCount == 2 )
                {
                    benchmark::RegisterBenchmark( benchName.c_str(), [noiseBenchmarkInterface, noiseType, dimensionSize2D]( benchmark::State& state )
                        {
                            noiseBenchmarkInterface->Benchmark2D( state, noiseType, dimensionSize2D );
                        } );
                }
                else if( dimensionCount == 3 )
                {
                    benchmark::RegisterBenchmark( benchName.c_str(), [noiseBenchmarkInterface, noiseType, dimensionSize3D]( benchmark::State& state )
                        {
                            noiseBenchmarkInterface->Benchmark3D( state, noiseType, dimensionSize3D );
                        } );
                }
            }
        }
    }

    benchmark::RunSpecifiedBenchmarks();

    std::cout << "Benchmarks Complete!";
    getchar();

    return 0;
}