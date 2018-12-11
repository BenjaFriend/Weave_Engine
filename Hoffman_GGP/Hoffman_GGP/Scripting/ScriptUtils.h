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
            LOG_TRACE( "Load Lua Entity: {} {} ", name, meshName );
           
            wideName = GetWC( aMeshName );

            Mesh* mesh = std::get<1>( ResourceManager::GetInstance()->LoadMesh( wideName ) );

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
            LOG_TRACE( "Unload Lua Entity: {}", name );
            createdEntity = nullptr;
            if ( wideName != nullptr )
            {
                delete wideName;
                wideName = nullptr;
            }
        }

        void SetPos( float x, float y, float z )
        {
            assert( createdEntity != nullptr );

            DirectX::XMFLOAT3 newPos( x, y, z );
            createdEntity->SetPosition( newPos );
        }

        std::string name;
        std::string meshName;
        std::string vertexShaderFile;
        std::string pixelShaderFile;


        Entity* createdEntity = nullptr;

    private:
        FileName wideName = nullptr;
    };

    struct MaterialCreationData
    {
        MaterialCreationData(
            SimpleVertexShader* vertexShader,
            SimplePixelShader* pixelShader,
            ID3D11DeviceContext* context,
            FileName albedoTexture,
            FileName normalTexture,
            FileName RoughnessTexture,
            FileName MetalTexture
        )
        {
            ResourceManager* resourceMan = ResourceManager::GetInstance();

            SRV_ID dif = resourceMan->LoadSRV( context, albedoTexture );
            SRV_ID normSRV = resourceMan->LoadSRV( context, normalTexture );
            SRV_ID roughnessMap = resourceMan->LoadSRV( context, RoughnessTexture );
            SRV_ID metalMap = resourceMan->LoadSRV( context, MetalTexture );

           /* Material_ID woodMatID = resourceMan->LoadMaterial( 
                vertexShader,
                pixelShader,
                dif,
                normSRV,
                roughnessMap,
                metalMap,
                SamplerID );*/

        }

        ~MaterialCreationData()
        {

        }
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

        void DefinedLuaTypes( sol::state & aLua );

        /** Lua update function callbacks */
        std::vector<sol::function> updateTicks;

        /** Lua states that should be persistent */
        std::vector<sol::state> luaStates;

        EntityManager* entityMan;

        ID3D11Device* device;

        ID3D11DeviceContext* context;
    };


}   // namespace Scripting
