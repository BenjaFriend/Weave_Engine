#include "Config.h"

#include "stdafx.h"

#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr
#include <fstream>
#include <assert.h>

#include "ServerUtils.h"
#include "WeaveServer.h"

int main( int argc, char* argv [] )
{
    Logger* logger = Logger::GetInstance();
    assert( logger != nullptr );
    LOG_TRACE( "Logger initlaized!" );

    SERVER_INIT_DESC serverDesc = {};

    // Parse command line arguments
    {
        for ( int i = 0; i < argc; ++i )
        {
            if ( strcmp( argv [ i ], HELP_FLAG ) == 0 )               // Help flag has been passed in
            {
                ServerUtils::PrintHelp();
                return 0;
            }
            else if ( strcmp( argv [ i ], CONFIG_FILE_FLAG ) == 0 )   // Specifying config file
            {
                ServerUtils::ParseConfigFile( serverDesc, argv [ i + 1 ] );
            }
        }
    }

    std::cout << serverDesc << std::endl;

    try
    {
        // Create the server 
        //std::unique_ptr< WeaveServer > Server =
        //    std::make_unique< WeaveServer >( serverDesc );

        WeaveServer Server( serverDesc );

        // run the server
        Server.Run();
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

    Logger::ReleaseInstance();
    logger = nullptr;

    return 0;
}