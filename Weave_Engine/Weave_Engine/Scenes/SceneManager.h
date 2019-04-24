#pragma once

#include <boost/filesystem.hpp>

#include "stdafx.h"
#include <iomanip>

#include "json/json.hpp"
#include "Utils/Dispatcher.hpp"
#include "Utils/SaveFileDefs.h"
#include "Scene.h"

namespace SceneManagement
{
    namespace fs = boost::filesystem;

    /// <summary>
    /// The scene manager will be able to load/unload 
    /// scene files and control the flow of what entities 
    /// and assets are currently loaded
    /// </summary>
    /// <author>Ben Hoffman</author>
    class SceneManager
    {
    public:

        /// <summary>
        /// Get an instance to the scene manager, if one does not 
        /// exist than one is created
        /// </summary>
        /// <returns>Pointer to the scene manager</returns>
        static SceneManager* GetInstance();

        /// <summary>
        /// Release the static instance of the scene manager.
        /// Unloads all current scene assets
        /// </summary>
        static void ReleaseInstance();

        /// <summary>
        /// Load a scene given a file, and create said entities
        /// appropriately. Unloads the current scene
        /// </summary>
        /// <param name="aSceneName">The scene file to load</param>
        void LoadScene( const FileName & aSceneName );

        /// <summary>
        /// Unload all currently loaded scene entities
        /// </summary>
        void UnloadCurrentScene();

        /// <summary>
        /// Save the current scene 
        /// </summary>
        /// <param name="aSceneFile"></param>
        void SaveScene( FileName & aSceneFile );

        /// <summary>
        /// Get the name of the currently loaded scene
        /// </summary>
        /// <returns>Name of the current scene</returns>
        FORCE_INLINE Scene* GetActiveScene() const { return ActiveScene; }

        /// <summary>
        /// The dispatcher for OnSceneLoad that you can use to add 
        /// listeners to this function
        /// </summary>
        /// <returns>Dispatcher reference for onScene load</returns>
        FORCE_INLINE Dispatcher & OnSceneLoad() { return OnSceneLoadDispatcher; }

        /// <summary>
        /// The dispatcher for OnSceneUnload that you can use to add
        /// listeners to this function
        /// </summary>
        /// <returns>Dispatacher reference for OnUnloadScene</returns>
        FORCE_INLINE Dispatcher & OnSceneUnload() { return OnSceneUnloadDispatcher; }

    private:

        SceneManager();

        ~SceneManager();

        /** Static instance of the scene manager */
        static SceneManager* Instance;

        /** A dispatcher to send events when the scene has loaded */
        Dispatcher OnSceneLoadDispatcher;

        /** A dispatcher that signals when the scene should be unloaded */
        Dispatcher OnSceneUnloadDispatcher;

        Scene * ActiveScene = nullptr;

    };
}   // namespace SceneManagement