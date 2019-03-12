#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr
#include <fstream>

#include <boost/asio.hpp>

#include "WeaveServer.h"

#define HELP_FLAG              "-h"
#define CONFIG_FILE_FLAG       "-c"

/**
* @brief   Prints out the help useage of this program.
*/
static void PrintHelp()
{
    std::cout << "Useage: ./Weave_Server [-h] [-c]" << std::endl;
    std::cout << "\t-h\tPrint out this useful help message" << std::endl;
    std::cout << "\t-c\tSpecify the server config file to use. If not specified the default values will be used." << std::endl;
}

/**
 * @brief               Attempt to read in the server config file.
 * @param aOutDesc      Server description to populate with config file data
 * @param aFileName     The config file name
*/
static void ParseConfigFile( SERVER_INIT_DESC & aOutDesc, const char* aFileName )
{
    assert( aFileName != nullptr );

    std::ifstream fileInput( aFileName );
    if( !fileInput.is_open() ) 
    {
        std::cerr << aFileName << " Was not a valid file! Cannot load in config." << std::endl;
        return;
    }

    for( std::string line; getline( fileInput, line ); )
    {
        std::string firstWord = line.substr(0, line.find(" "));
        if( firstWord == "ServerName" )
        {
            char name[ 64 ];
            sscanf( line.c_str(), "ServerName : %s", name );
        }
        else if( firstWord == "ListenPort" )
        {
            sscanf( line.c_str(), "ListenPort : %hu", &aOutDesc.ListenPort );
        }
        else if( firstWord == "ResponsePort" )
        {
            sscanf( line.c_str(), "ResponsePort : %hu", &aOutDesc.ResponsePort );
        }
        else if( firstWord == "MaxRooms" )
        {
            sscanf( line.c_str(), "MaxRooms : %hu", &aOutDesc.MaxRooms ); 
        }
    }

    fileInput.close();
}

int main( int argc, char* argv [] )
{
    SERVER_INIT_DESC serverDesc = {};

    // Loop through the args that are passed in
    for( int i = 0; i < argc; ++i )
    {
        if( strcmp( argv[ i ], HELP_FLAG ) == 0 )               // Help flag has been passed in
        {
            PrintHelp();
            return 0;
        }
        else if( strcmp( argv[ i ], CONFIG_FILE_FLAG ) == 0 )   // Specifying config file
        {
            ParseConfigFile( serverDesc, argv[ i + 1 ] );
        }
    }

    std::cout << serverDesc << std::endl;

    try
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


    std::cout << "Program exit!\n\n" << std::endl;

    return 0;
}
