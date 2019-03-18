#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr
#include <fstream>
#include <unordered_map>

#include <boost/asio.hpp>

#include "WeaveServer.h"

#define HELP_FLAG              "-h"
#define CONFIG_FILE_FLAG       "-c"

enum class EUserCoptions : uint8_t
{
    QUIT,
    HELP
};

static std::unordered_map<std::string, EUserCoptions> UserOptions;

/**
* @brief   Prints out the help usage of this program.
*/
static void PrintHelp()
{
    std::cout << "Usage: ./Weave_Server [-h] [-c]" << std::endl;
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
    if ( !fileInput.is_open() )
    {
        std::cerr << aFileName << " Was not a valid file! Cannot load in config." << std::endl;
        return;
    }

    for ( std::string line; getline( fileInput, line ); )
    {
        std::string firstWord = line.substr( 0, line.find( " " ) );

        if ( firstWord == "ListenPort" )
        {
            sscanf( line.c_str(), "ListenPort : %hu", &aOutDesc.ListenPort );
        }
        else if ( firstWord == "ResponsePort" )
        {
            sscanf( line.c_str(), "ResponsePort : %hu", &aOutDesc.ResponsePort );
        }
        else if ( firstWord == "MaxRooms" )
        {
            sscanf( line.c_str(), "MaxRooms : %hu", &aOutDesc.MaxRooms );
        }
    }

    fileInput.close();
}

/// <summary>
/// Initialize the list of commands that the user can use during the server runtime.
/// </summary>
static void InitalizeCommands()
{
    UserOptions [ "QUIT" ] = EUserCoptions::QUIT;
    UserOptions [ "HELP" ] = EUserCoptions::HELP;
}

int main( int argc, char* argv [] )
{
    SERVER_INIT_DESC serverDesc = {};

    // Loop through the args that are passed in
    for ( int i = 0; i < argc; ++i )
    {
        if ( strcmp( argv [ i ], HELP_FLAG ) == 0 )               // Help flag has been passed in
        {
            PrintHelp();
            return 0;
        }
        else if ( strcmp( argv [ i ], CONFIG_FILE_FLAG ) == 0 )   // Specifying config file
        {
            ParseConfigFile( serverDesc, argv [ i + 1 ] );
        }
    }

    std::cout << serverDesc << std::endl;
    
    InitalizeCommands();

    try
    {
        std::unique_ptr< WeaveServer > Server =
            std::make_unique< WeaveServer >( serverDesc );

        std::string input = "";
        bool isDone = false;
        // Input loop for stopping the server
        while ( !isDone )
        {
            std::cout << "Enter a command [ QUIT ]: ";
            std::cin >> input;
            std::string cmd;

            // Put the input to all upper case into the "cmd" variable
            std::transform( input.begin(), input.end(), std::back_inserter( cmd ), ::toupper );

            switch ( UserOptions [ cmd ] )
            {
                case EUserCoptions::QUIT:
                    std::cout << "Quitting..." << std::endl;
                    Server->Shutdown();
                    isDone = true;
                break;

                case EUserCoptions::HELP:
                    std::cout << "Command options: " << std::endl;
                    for ( auto const & possibleCmd : UserOptions )
                    {
                        std::cout << "\t" << possibleCmd.first << std::endl;
                    }
                    std::cout << "\n\n" << std::endl;
                break;
            default:
                std::cout << "\t" << cmd << " is not a valid command!" << std::endl;
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
