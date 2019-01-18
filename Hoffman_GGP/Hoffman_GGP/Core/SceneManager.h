#pragma once

#include "../stdafx.h"

namespace SceneManagement
{
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
        /// appropriately
        /// </summary>
        /// <param name="aSceneName">The scene file to load</param>
        void LoadScene( FileName & aSceneName );

        /// <summary>
        /// Unload all currently loaded scene entities
        /// </summary>
        void UnloadCurrentScene();

        /// <summary>
        /// Get the name of the currently loaded scene
        /// </summary>
        /// <returns>Name of the current scene</returns>
        const FileName GetActiveScene() const { return ActiveScene; }

    private:

        SceneManager();

        ~SceneManager();

        /** Static instance of the scene manager */
        static SceneManager* Instance;
        
        /** The current scene that is loaded in */
        FileName ActiveScene;
    };
}   // namespace SceneManagement