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

#define DLL_EXPORT
#include <NoiseBenchmarkInterface.h>

std::vector<NoiseBenchmarkInterface*> NoiseBenchmarkInterfaces;

void RegisterNoiseBenchmarkInterface( NoiseBenchmarkInterface* p )
{
    NoiseBenchmarkInterfaces.push_back( p );
}

bool LoadDynamicLibs()
{
    std::string libPath( "." );

    if( libPath.empty() )
    {//find where we are loaded from and look in that directory
        libPath = "./";
#if defined(_WIN32) || defined(_WIN64)
        HMODULE hModule = NULL;

        if( GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&LoadDynamicLibs, &hModule ) )
        {
            char path[MAX_PATH];

            if( GetModuleFileName( hModule, path, sizeof( path ) ) > 0 )
            {
                fs::path imagePath( path );
                libPath = imagePath.parent_path().string();
            }
        }
#elif defined(__APPLE__)
#ifdef NDEBUG
        std::string libName = "libhastyNoise";
#else
        std::string libName = "libhastyNoised";
#endif
        uint32_t dyldCount = _dyld_image_count();

        for( uint32_t i = 0; i < dyldCount; ++i )
        {
            const char* dyldImageName = _dyld_get_image_name( i );

            if( dyldImageName == NULL )
                continue;

            fs::path imagePath( dyldImageName );

            if( imagePath.stem() == libName )
            {
                libPath = imagePath.parent_path().string();
                break;
            }
        }
#elif defined( __linux__)
        dl_iterate_phdr( phdr_callback, &libPath );
#else
        static_assert(false);
#endif
    }

    fs::path directory( libPath );

    if( !fs::exists( directory ) || !fs::is_directory( directory ) )
        return false;

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

    return true;
}

int main( int argc, char** argv )
{
    LoadDynamicLibs();

    for( size_t dimensions = 2; dimensions <= 2; dimensions++ )
    {
        for( auto noiseType = (NoiseBenchmarkInterface::NoiseType)0;
             noiseType < NoiseBenchmarkInterface::NoiseType::EnumMax;
             noiseType = (NoiseBenchmarkInterface::NoiseType)((int)noiseType + 1) )
        {
            for( NoiseBenchmarkInterface* noiseBenchmarkInterface : NoiseBenchmarkInterfaces )
            {
                std::string benchName = noiseBenchmarkInterface->FormatBenchmarkName( noiseType, dimensions );

                noiseBenchmarkInterface->RegisterBenchmark2D( benchName.c_str(), noiseType, dimensions );
            }
        }
    }

    benchmark::RunSpecifiedBenchmarks();

    getchar();

    return 0;
}