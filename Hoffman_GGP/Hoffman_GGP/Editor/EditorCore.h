#pragma once

#include "../Entity/EntityManager.h"
#include "../ECS/ComponentManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/SimpleShader.h"
#include "../Resources/Vertex.h"
#include "../Entity/Camera.h"

#include "json/json.hpp"
#include <iomanip>

#if defined ( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#endif

#if defined ( _WIN32 ) || defined ( _WIN64 )

#include <d3d11.h>
#include <DirectXMath.h>

#endif

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
        void Update( float dt );

        void Draw(
            float dt,
            ID3D11Device* aDevice,
            ID3D11DeviceContext* aContext
        );

        /// <summary>
        /// Save all entities to a scene json file 
        /// </summary>
        void SaveScene();

        /// <summary>
        /// Loads entities from the scene json file 
        /// </summary>
        void LoadScene();

        /// <summary>
        /// Get the currently selected entity by by the user
        /// </summary>
        /// <returns>Pointer to the selected entity</returns>
        Entity* GetSelectedEntity() const { return SelectedEntity; }

        //////////////////////////////////////////////////////////////////
        // Accessors 
        //////////////////////////////////////////////////////////////////

        const bool GetDrawLightGizmos() const { return DrawLightGizmos; }
        void SetDrawLightGizmos( const bool aDrawGizmos );

        const bool GetDrawSkybox() const { return DrawSkyBox; }
        void SetDrawSkybox( const bool aDrawBox ) { DrawSkyBox = aDrawBox; }

        const bool GetDrawColliders() const { return DebugDrawColliders; }
        void SetDrawColliders( const bool aDrawColliders ) { DebugDrawColliders = aDrawColliders; }

        void SetSceneFile( const FileName & aFileName );
        const FileName & GetSceneFileName() const { return SceneFile; }

        void SetCamera( Camera* aCam ) { CurrentCamera = aCam; }
        Camera* GetCamera()const { return CurrentCamera; }

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

        /** Resource manager for handling any editor-specific shaders */
        ResourceManager* resourceMan = nullptr;

        /** The outline shader that can be used for the selected object */
        SimplePixelShader* OutlineShader = nullptr;

        /** Vertex shader to use for drawing any gizmos */
        SimpleVertexShader* VertexShader = nullptr;
        
        /** Wireframe state for drawing gizmos */
        ID3D11RasterizerState* WireFrame = nullptr;

        /** The current editor camera */
        Camera* CurrentCamera = nullptr;

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
        FileName SceneFile = L"Scene_test.json";

    };

}   // namespace Editor