#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

#include "../Entity/EntityManager.h"
#include "../Resources/ResourceManager.h"

namespace Scripting
{
    // Create the entity type
    struct EntityCreationData
    {
        EntityCreationData( const char* aName, const char* aMeshName )
            : name( aName ), meshName( aMeshName )
        {
            // Tell the entity manager to create an Entity with
            LOG_WARN( "Hello Entity, {} {} ", name, meshName );
            Mesh_ID meshID = ResourceManager::GetInstance()->LoadMesh( meshName.c_str() );
            Mesh* mesh = ResourceManager::GetInstance()->GetMesh( meshID );

            // Create the entity in the entity manager
            Entity_ID id = EntityManager::GetInstance()->AddEntity( 
                mesh,
                ResourceManager::GetInstance()->GetMaterial( 0 ),
                name 
            );

            createdEntity = EntityManager::GetInstance()->GetEntity( id );
        }

        ~EntityCreationData()
        {
            LOG_WARN( "Bye Entity" );
            createdEntity = nullptr;
        }

        void SetPos( float x, float y, float z )
        {
            DirectX::XMFLOAT3 newPos ( x, y, z );
            createdEntity->SetPosition( newPos );
        }

        std::string name;
        std::string meshName;

        Entity* createdEntity = nullptr;
    };


    void TestScripting();


    class ScriptManager
    {
    public:

        ScriptManager( ID3D11Device* aDevice, ID3D11DeviceContext* aContext );

        ~ScriptManager();

        /// <summary>
        /// Update any entity callbacks
        /// </summary>
        /// <param name="deltaTime"></param>
        void Update( float deltaTime );

        /// <summary>
        /// Load all scripts in the Assets/Scripts directory
        /// </summary>
        void LoadScripts();

    private:

        /// <summary>
        /// Load in this lua script and store it's lua state
        /// </summary>
        /// <param name="aFile"></param>
        void LoadScript( const char* aFile );

        /** Lua update function callbacks */
        std::vector<sol::function> updateTicks;

        /** Lua states that should be persistant */
        std::vector<sol::state> luaStates;

        EntityManager* entityMan;

        ID3D11Device* device;

        ID3D11DeviceContext* context;
    };

}   // namespace Scripting
