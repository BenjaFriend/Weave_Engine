#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include <filesystem>   // std::filesystem::directory_iterator

#include "../Scenes/SceneManager.h"
#include "../Core/CameraManager.h"
#include "../Scenes/Scene.h"
#include "../Entity/Entity.h"
#include "../Entity/Transform.h"
#include "../Entity/Camera.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/MeshRenderer.h"

namespace Scripting
{
    /// <summary>
    /// Class for loading in all scripts in the assets folder
    /// </summary>
    /// <author>Ben Hoffman</author>
    class ScriptManager
    {
    public:

        ScriptManager();

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
        void DefineLuaTypes( sol::state & aLua );

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

        /// <summary>
        /// Load in an entity with the given info about it from a table
        /// </summary>
        /// <param name="aEntityInfo">Sol table from Lua</param>
        /// <returns>Pointer to the created entity</returns>
        Entity* CreateEntity( const sol::table & aEntityInfo );

		void Log_Print(std::string msg);

		void MoveCamera(glm::vec3 move, glm::vec2 mouseMove);

        /** Lua update function callbacks */
        std::vector<sol::function> UpdateTicks;

        /** Lua on click function callbacks */
        std::vector<sol::function> OnClickCallbacks;

        /** Lua states that should be persistent */
        std::vector<sol::state> LuaStates;

        /** Vector of script file paths */
        std::vector<std::string> ScriptPaths;

    };

}   // namespace Scripting