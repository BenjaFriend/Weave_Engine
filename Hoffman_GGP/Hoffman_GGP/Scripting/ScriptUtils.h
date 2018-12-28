#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include <filesystem>

#include "../Entity/EntityManager.h"
#include "../Resources/ResourceManager.h"

namespace Scripting
{
    /// <summary>
    /// Class for loading in all scripts in the assets folder
    /// </summary>
    /// <author>Ben Hoffman</author>
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
        /// Send out on click callbacks to Lua scripts
        /// </summary>
        void OnClick();

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

        /// <summary>
        /// Read a given directory and add any files in it to the given vector
        /// </summary>
        /// <param name="dirName">The directory to search</param>
        /// <param name="aPathVec">vector of file paths to add to</param>
        void ReadDirectory( const std::string& dirName, std::vector<std::string>& aPathVec );

        /// <summary>
        /// Checks to see if there is a callback function of this name
        /// in the given lua state, if there is, then push to the vector
        /// of callbacks.
        /// </summary>
        /// <param name="lua">The lua state to check</param>
        /// <param name="aFuncName">function name to check for</param>
        /// <param name="aCallbackVec">Vector of function callbacks to add to</param>
        void AddCallback( 
            const sol::state & lua,
            const char* aFuncName, 
            std::vector<sol::function>& aCallbackVec 
        );

        /// <summary>
        /// Run a function in the lua state if it exists
        /// </summary>
        /// <param name="lua">Lua state to run the function in</param>
        /// <param name="aFuncName">Name of the function to run</param>
        void RunLuaFunction( const sol::state & lua, const char* aFuncName );

        /** Lua update function callbacks */
        std::vector<sol::function> UpdateTicks;

        /** Lua on click function callbacks */
        std::vector<sol::function> OnClickCallbacks;

        /** Lua states that should be persistent */
        std::vector<sol::state> LuaStates;

        /** Target device to create shaders/materials on */
        ID3D11Device* Device;

        /** Target context to create shaders/materials on */
        ID3D11DeviceContext* Context;

        /** Vector of script file paths */
        std::vector<std::string> ScriptPaths;

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
