#include <iostream>
#include <string>       // std::string
#include <thread>       // std::thread
#include <memory>       // std::unique_ptr

#include <boost/asio.hpp>

#include "WeaveServer.h"

#define HELP_FLAG              "-h"

int main( int argc, char* argv [] )
{
    if ( argc <= 1 )
    {
        std::cout << "There is only one argument at argv[0]: \n\t"
            << argv [ 0 ] << std::endl;
    }

    // TODO: Parse args for server config
    SERVER_INIT_DESC serverDesc = {};

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


    std::cout << "Program exit! " << std::endl;

    return 0;
}
