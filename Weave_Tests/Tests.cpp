/// <summary>
/// A test unit test for configuring Catch2
///</summary>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch2/catch.hpp"

int my_add( int a, int b ) 
{
	return ( a + b );
} 


TEST_CASE( "inital test case", "[factorial]" )
{
    REQUIRE( 4 == my_add( 2, 2 ) );
}
