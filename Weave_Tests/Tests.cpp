/// <summary>
/// A test unit test for configuring Catch2
///</summary>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch2/catch.hpp"
#include "stdafx.h"
#include "MemoryBitStream.h"

TEST_CASE ( "InputMemoryBitStream", "[InputMemoryBitStream]" )
{
    char buf [ 64 ] = {};

    InputMemoryBitStream stream( buf, 64 );

    REQUIRE( stream.GetBufferPtr() != nullptr );
}

