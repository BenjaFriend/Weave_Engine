#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <thread>

#include "Test_Include.h"
#include "Funcs.h"

#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

int main( int argc, char* argv[] )
{
    if( argc <= 1 )
    {
        std::cout << "There is only one argument at argv[0]"
        << argv[ 0 ] << std::endl;
    }
    std::cout << "Hello world! Initalize asio and boost please!" << std::endl;
    TestFunc();
    return 0;
}
