#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include <filesystem>   // std::filesystem::directory_iterator

#include "../Scenes/SceneManager.h"
#include "../Camera/CameraManager.h"
#include "../Scenes/Scene.h"
#include "../Entity/Entity.h"
#include "Entity/Transform.h"
#include "../Camera/Camera.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/MeshRenderer.h"

namespace Scripting
{
    struct ScriptBehaviors
    {
        sol::state ScriptState;     // the sol state of this script
        sol::function UpdateFunc    = sol::nil;   // the update fnuction callback
        sol::function StartFunc     = sol::nil;    // the start function callback
        sol::function OnClickfun    = sol::nil;   // The onClickFunction callback
    };

    /// <summary>
    /// Class for loading in all scripts in the assets folder
    /// </summary>
    /// <author>Ben Hoffman</author>
    class ScriptManager
    {
    public:

        static ScriptManager* GetInstance();

        static void ReleaseInstance();

        /// <summary>
        /// Run all current scripts that have a Start callback
        /// </summary>
        void Start();

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
        /// Register a script and it's behaviors to the manager
        /// </summary>
        /// <param name="aFileName">The file path to the script</param>
        void RegisterScript( const std::string & aFileName );

        /// <summary>
        /// Release the script behavior from the manager
        /// </summary>
        /// <param name="aFileName">The file path to the script</param>
        void ReleaseScript( const std::string & aFileName );

    private:

        ScriptManager();

        ~ScriptManager();

        static ScriptManager* Instance;

        /// <summary>
        /// Load in this lua script and store it's lua state
        /// </summary>
        /// <param name="aFile"></param>
        void LoadScript( const char* aFile, ScriptBehaviors * aOutBehavior );

        /// <summary>
        /// Define the lua states for any game play scripts
        /// </summary>
        /// <param name="aLua">the lua state to edit</param>
        void DefineLuaTypes( sol::state & aLua );

        /// <summary>
        /// Checks to see if there is a callback function of this name
        /// in the given lua state, if there is, then push to the vector
        /// of callbacks.
        /// </summary>
        /// <param name="lua">The lua state to check</param>
        /// <param name="aFuncName">function name to check for</param>
        /// <param name="aCallbackVec">Vector of function callbacks to add to</param>
        void AddCallback( const sol::state & lua, const char* aFuncName, sol::function * aOutCallbackVec );

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

        /// <summary>
        /// A bind for lua to use the debug print 
        /// </summary>
        /// <param name="msg"></param>
        void Log_Print( const std::string & msg );

        /// <summary>
        /// Update the active camera's transform position and rotation
        /// </summary>
        /// <param name="move">The relative movement change of the camera</param>
        /// <param name="rotate">The relative rotation change of the camera</param>
        void MoveCamera( glm::vec3 & move, glm::vec2 & rotate );

        /** A map of file locations to asset behaviors */
        std::unordered_map < std::string, ScriptBehaviors > LuaBehaviors;
    };

}   // namespace Scripting