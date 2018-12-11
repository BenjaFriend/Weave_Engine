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
        {
            LOG_TRACE( "Load Lua Entity: {} {} ", aName, aMeshName );

            WideName = GetWC( aMeshName );

            Mesh* mesh = std::get<1>( ResourceManager::GetInstance()->LoadMesh( WideName ) );

            // Create the entity in the entity manager
            Entity_ID id = EntityManager::GetInstance()->AddEntity(
                mesh,
                ResourceManager::GetInstance()->GetMaterial( 0 ),
                aName
            );

            CreatedEntity = EntityManager::GetInstance()->GetEntity( id );
        }

        ~EntityCreationData()
        {
            CreatedEntity = nullptr;

            SAFE_DELETE( WideName );
        }

        void SetPos( float x, float y, float z )
        {
            assert( CreatedEntity != nullptr );

            DirectX::XMFLOAT3 newPos( x, y, z );
            CreatedEntity->SetPosition( newPos );
        }

    private:

        /** Info to create the entity */
        std::string VertexShaderFile;
        std::string PixelShaderFile;

        /** Pointer to the entity that was created */
        Entity* CreatedEntity = nullptr;

        /** The file name of this created entity in wide format */
        FileName WideName = nullptr;
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

        /// <summary>
        /// Define the lua states for any game play scripts
        /// </summary>
        /// <param name="aLua">the lua state to edit</param>
        void DefinedLuaTypes( sol::state & aLua );

        /** Lua update function callbacks */
        std::vector<sol::function> UpdateTicks;

        /** Lua states that should be persistent */
        std::vector<sol::state> LuaStates;

        /** Target device to create shaders/materials on */
        ID3D11Device* Device;

        /** Target context to create shaders/materials on */
        ID3D11DeviceContext* Context;
    };


}   // namespace Scripting
