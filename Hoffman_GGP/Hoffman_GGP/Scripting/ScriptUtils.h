#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

#include "../Entity/EntityManager.h"
#include "../Resources/ResourceManager.h"

namespace Scripting
{
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


        /***********************************************************/
        /* Creation data definitions                               */
        /***********************************************************/

        struct MaterialCreationData
        {
            MaterialCreationData(
                ID3D11Device* aDevice,
                ID3D11DeviceContext* aContext,
                const char* vertexShader,
                const char* pixelShader,
                const char* albedoTexture,
                const char* normalTexture,
                const char* RoughnessTexture,
                const char* MetalTexture
            )
            {
                LOG_WARN( " Create a material: {} {}", aDevice != nullptr, aContext != nullptr );
                ResourceManager* resourceMan = ResourceManager::GetInstance();

                const wchar_t* vsW = GetWC( vertexShader );
                const wchar_t* psW = GetWC( pixelShader );
                const wchar_t* albedoW = GetWC( albedoTexture );
                const wchar_t* normalW = GetWC( normalTexture );
                const wchar_t* roughW = GetWC( RoughnessTexture );
                const wchar_t* MetalW = GetWC( MetalTexture );

                SimpleVertexShader* vs = resourceMan->LoadShader<SimpleVertexShader>(
                    aDevice,
                    aContext,
                    vsW );

                SimplePixelShader* ps = resourceMan->LoadShader<SimplePixelShader>(
                    aDevice,
                    aContext,
                    psW );
                
                assert( ps && vs );

                SRV_ID dif = resourceMan->LoadSRV( aContext, albedoW );
                SRV_ID normSRV = resourceMan->LoadSRV( aContext, normalW );
                SRV_ID roughnessMap = resourceMan->LoadSRV( aContext, roughW );
                SRV_ID metalMap = resourceMan->LoadSRV( aContext, MetalW );

                materialID = resourceMan->LoadMaterial(
                    vs,
                    ps,
                    dif,
                    normSRV,
                    roughnessMap,
                    metalMap,
                    0 );   // Use default sampler
                
                // #TODO 
                // Figure out what the deal with wide chars and lua is because this sucks
                SAFE_DELETE( vsW );
                SAFE_DELETE( psW );
                SAFE_DELETE( albedoW );
                SAFE_DELETE( normalW );
                SAFE_DELETE( roughW );
                SAFE_DELETE( MetalW );
            }

            ~MaterialCreationData() {}

            Material_ID materialID;
        };

        // Create the entity type
        struct EntityCreationData
        {
            EntityCreationData( const char* aName, const char* aMeshName, MaterialCreationData* matData )
            {
                LOG_TRACE( "Load Lua Entity: {} {} ", aName, aMeshName );

                WideName = GetWC( aMeshName );

                ResourceManager* resMan = ResourceManager::GetInstance();

                Mesh* mesh = std::get<1>( resMan->LoadMesh( WideName ) );

                // Create the entity in the entity manager
                Entity_ID id = EntityManager::GetInstance()->AddEntity(
                    mesh,
                    resMan->GetMaterial( matData->materialID ),
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

    };


}   // namespace Scripting
