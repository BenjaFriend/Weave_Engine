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
                FileName vertexShader,
                FileName pixelShader,
                FileName albedoTexture,
                FileName normalTexture,
                FileName roughnessTexture,
                FileName metalTexture
            )
            {
                ResourceManager* resourceMan = ResourceManager::GetInstance();

                SimpleVertexShader* vs = resourceMan->LoadShader<SimpleVertexShader>(
                    aDevice,
                    aContext,
                    vertexShader );

                SimplePixelShader* ps = resourceMan->LoadShader<SimplePixelShader>(
                    aDevice,
                    aContext,
                    pixelShader );
                
                assert( ps && vs );

                SRV_ID dif = resourceMan->LoadSRV( aContext, albedoTexture);
                SRV_ID normSRV = resourceMan->LoadSRV( aContext, normalTexture );
                SRV_ID roughnessMap = resourceMan->LoadSRV( aContext, roughnessTexture );
                SRV_ID metalMap = resourceMan->LoadSRV( aContext, metalTexture );

                materialID = resourceMan->LoadMaterial(
                    vs,
                    ps,
                    dif,
                    normSRV,
                    roughnessMap,
                    metalMap,
                    0 );   // Use default sampler
            }

            ~MaterialCreationData() {}

            Material_ID materialID;
        };

        // Create the entity type
        struct EntityCreationData
        {
            EntityCreationData( std::string aName, FileName aMeshName, MaterialCreationData* matData )
            {
                LOG_TRACE( "Load Lua Entity: {} ", aName );

                MeshName = aMeshName;

                ResourceManager* resMan = ResourceManager::GetInstance();

                Mesh* mesh = std::get<1>( resMan->LoadMesh( MeshName ) );

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
            }

            void SetPos( const float x, const float y, const float z )
            {
                assert( CreatedEntity != nullptr );

                DirectX::XMFLOAT3 newPos( x, y, z );
                CreatedEntity->SetPosition( newPos );
            }

            void SetScale( const float x, const float y, const float z )
            {
                assert( CreatedEntity != nullptr );

                DirectX::XMFLOAT3 newScale( x, y, z );
                CreatedEntity->SetScale( newScale );
            }

            void SetMass( const float aVal )
            {
                assert( CreatedEntity != nullptr );
                CreatedEntity->SetMass( aVal );
            }

            const float GetMass() const
            {
                assert( CreatedEntity != nullptr );
                return CreatedEntity->GetMass();
            }

        private:

            /** Info to create the entity */
            std::string VertexShaderFile;
            std::string PixelShaderFile;

            /** Pointer to the entity that was created */
            Entity* CreatedEntity = nullptr;

            /** The file name of this created entity in wide format */
            FileName MeshName;
        };

    };


}   // namespace Scripting
