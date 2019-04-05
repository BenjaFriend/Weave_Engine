/// <summary>
/// A test unit test for configuring Catch2
///</summary>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch2/catch.hpp"
#include "stdafx.h"

#include "Utils/ObjectPool.hpp"

#include "MemoryBitStream.h"

TEST_CASE ( "OutputMemoryBitStream", "[OutputMemoryBitStream]" )
{
    char buf [ 64 ] = {};

    InputMemoryBitStream stream( buf, 64 );

    REQUIRE( stream.GetBufferPtr() != nullptr );
}


TEST_CASE ( "Object Pool Get", "[ObjectPool]" )
{
    const size_t size = 4;

    ObjectPool< int > pool( size );

    SECTION( "Size checking" )
    {
        REQUIRE( pool.GetNumberAvailableResources() == size );
        REQUIRE( pool.IsEmpty() == false );
    }

    SECTION( "Null checks" )
    {
        int* res = pool.GetResource();
        REQUIRE( res != nullptr );
    }
}
