#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr

#include <boost/asio.hpp>

#include "WeaveServer.h"

#define HELP_FLAG              "-h"

int main( int argc, char* argv[] )
{
    if( argc <= 1 )
    {
        std::cout << "There is only one argument at argv[0]"
        << argv[ 0 ] << std::endl;
    }

    // TODO: Parse args for server config

    std::cout << "Hello world! Initalize asio and boost please!" << std::endl;

    SERVER_INIT_DESC serverDesc = {};

    std::unique_ptr< WeaveServer > Server =
        std::make_unique< WeaveServer > ( serverDesc );
    
    std::cout << "Program exit! " << std::endl;

    return 0;
}
