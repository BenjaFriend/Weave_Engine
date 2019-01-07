#pragma once

#include <DirectXMath.h>

#include "../Entity/EntityManager.h"
#include "../ECS/ComponentManager.h"


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

    private:

        EditorCore();

        ~EditorCore();

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

        EntityManager* entityMan = nullptr;

        Entity* SelectedEntity = nullptr;

    };

}   // namespace Editor