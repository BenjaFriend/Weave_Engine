#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

namespace Scripting
{
    // Create the entity type
    struct EntityCreationData
    {
        EntityCreationData() { LOG_WARN( "Hello Entity" ); }
        ~EntityCreationData() { LOG_WARN( "Bye Entity" ); }
        int data;
        std::string file;
    };


    void TestScripting();


    void LoadSceneFile( ID3D11Device* aDevice, const char * aFile );


}   // namespace Scripting
