#pragma once

#include <DirectXMath.h>

#include "../Entity/EntityManager.h"
#include "../ECS/ComponentManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "json/json.hpp"
#include <iomanip>

namespace Editor
{

    /// <summary>
    /// Core functionality of the editor for editing a scene
    /// </summary>
    /// <author>Ben Hoffman</author>
    class EditorCore
    {
    public:

        /// <summary>
        /// Get an instance of the editor, if one exists. 
        /// If one doesnt exist, then create one
        /// </summary>
        /// <returns>Pointer to the static editor instance</returns>
        static EditorCore* GetInstance();

        /// <summary>
        /// Destroy the editor instance if it exists
        /// </summary>
        static void ReleaseInstance();

        /// <summary>
        /// Update the editor for a frame
        /// </summary>
        /// <param name="dt">Delta Time of this frame</param>
        void Update();

        /// <summary>
        /// Save all entities to a scene json file 
        /// </summary>
        void SaveScene();

        /// <summary>
        /// Loads entities from the scene json file 
        /// </summary>
        void LoadScene();

    private:

        EditorCore();

        ~EditorCore();

        /// <summary>
        /// Load in any resources (shaders, meshes, etc) that the 
        /// editor may need
        /// </summary>
        void LoadResources();

        /// <summary>
        /// Draw the editor UI
        /// </summary>
        void DrawUI();

        /// <summary>
        /// Draw any gizmos that should be on screen
        /// </summary>
        void DrawGizmos();

        /** Static instance of the editor */
        static EditorCore* Instance;

        /** Flag to set lighting gizmos */
        bool DrawLightGizmos = true;

        /** Flag to draw the skybox */
        bool DrawSkyBox = true;

        /** Flag to draw colliders or not */
        bool DebugDrawColliders = true;

        /** Entity manager pointer for editing entity data */
        EntityManager* entityMan = nullptr;

        /** The entity that the user wants to change */
        Entity* SelectedEntity = nullptr;

        /** The current scene file to save all entities to */
        char SceneFile [ 64 ] = "Scene_test.json";

    };

}   // namespace Editor