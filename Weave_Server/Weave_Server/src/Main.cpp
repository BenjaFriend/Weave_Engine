#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr

#include <boost/asio.hpp>

#include "WeaveServer.h"

#define HELP_FLAG              "-h"
#define CONFIG_FILE_FLAG       "-c"

static void PrintHelp()
{
    std::cout << "Useage: ./Weave_Server [-h] [-c]" << std::endl;
    std::cout << "\t-h\tPrint out this useful help message" << std::endl;
    std::cout << "\t-c\tSpecify the server config file to use. If not specified the default values will be used." << std::endl;
}

static void ParseConfigFile( SERVER_INIT_DESC & aOutDesc, const char* aFileName )
{
    assert( aFileName != nullptr );

    FILE* file = fopen( aFileName, "r" );
    if( file == nullptr )
    {
        std::cerr << aFileName << "Is not a valid config file name!" << std::endl;
        return;
    }  

    // Read to the end of the file
    char buf[ 64 ];
    while( fgets( buf, 64, file ) != NULL )
    {
        printf( "%s", buf );
    }

    if( file != nullptr )
    {
        fclose( file );
        file = nullptr;
    }
}

int main( int argc, char* argv [] )
{
    SERVER_INIT_DESC serverDesc = {};

    // Loop through the args that are passed in
    for( int i = 0; i < argc; ++i )
    {
        if( strcmp( argv[ i ], HELP_FLAG ) == 0 )
        {
            PrintHelp();
            return 0;
        }
        else if( strcmp( argv[ i ], CONFIG_FILE_FLAG ) == 0 )
        {
            ParseConfigFile( serverDesc, argv[ i + 1 ] );
        }
    }

    std::cout << serverDesc << std::endl;

   /* try
    {   
        std::unique_ptr< WeaveServer > Server =
            std::make_unique< WeaveServer >( serverDesc );

        std::string input = "";
        // Input loop for stopping the server
        while ( 1 )
        {
            std::cout << "Enter a command [ QUIT ]: ";
            std::cin >> input;
            std::cout << "\tYou selected: " << input << std::endl;

            if ( input == "QUIT" )
            {
                Server->Shutdown();
                break;
            }

        }

    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
*/

    std::cout << "Program exit!\n\n" << std::endl;

    return 0;
}
