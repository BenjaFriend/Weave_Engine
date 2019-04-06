#pragma once

#include "../Entity/Entity.h"
#include "ECS/ComponentManager.h"
#include "../Resources/ResourceManager.h"
#include "../Scenes/SceneManager.h"
#include "../Resources/SimpleShader.h"
#include "../Resources/Vertex.h"
#include "../Resources/MeshRenderer.h"
#include "../Camera/CameraManager.h"

#include "Gizmo.h"

#include "json/json.hpp"
#include <iomanip>

#if defined ( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#endif

#include "MathHelper.h"

#include "Utils/SaveFileDefs.h"

namespace Editor
{

    /// <summary>
    /// Core functionality of the editor for editing a scene
    /// Including most IMGUI application
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

        /// <summary>
        /// Draw any editor gizmos, editor UI, and highlight
        /// the currently selected entity
        /// </summary>
        /// <param name="dt">Delta time</param>
        /// <param name="aDevice">DX11 device</param>
        /// <param name="aContext">DX11 context</param>
        void Draw(
            float dt,
            ID3D11Device* aDevice,
            ID3D11DeviceContext* aContext
        );

        /// <summary>
        /// Save all entities to a scene json file 
        /// The files is based on the current scene file from the editor window
        /// </summary>
        void SaveScene();

        /// <summary>
        /// Loads entities from the scene json file based on the currently
        /// selected scene file name
        /// </summary>
        void LoadScene();

        //////////////////////////////////////////////////////////////////
        // Accessors 
        //////////////////////////////////////////////////////////////////

        /// <summary>
        /// Get the currently selected entity by by the user
        /// </summary>
        /// <returns>Pointer to the selected entity</returns>
        FORCE_INLINE Entity* GetSelectedEntity() const { return SelectedEntity; }

        FORCE_INLINE const bool GetDrawGizmos() const { return DoGizmoDraw; }
        FORCE_INLINE void SetDrawGizmos( const bool aDrawGizmos );

        FORCE_INLINE const bool GetDrawSkybox() const { return DrawSkyBox; }
        FORCE_INLINE void SetDrawSkybox( const bool aDrawBox ) { DrawSkyBox = aDrawBox; }

        FORCE_INLINE const bool GetDrawColliders() const { return DebugDrawColliders; }
        FORCE_INLINE void SetDrawColliders( const bool aDrawColliders ) { DebugDrawColliders = aDrawColliders; }

        FORCE_INLINE void SetSceneFile( const FileName & aFileName );
        FORCE_INLINE const FileName & GetSceneFileName() const { FileName fName( SceneFile.begin(), SceneFile.end() ); return fName; }

    private:

        EditorCore();

        ~EditorCore();

        /// <summary>
        /// Load in any resources (shaders, meshes, etc) that the 
        /// editor may need
        /// </summary>
        void LoadResources();

        /// <summary>
        /// Will draw the point light circles if enabled
        /// </summary>
        void DrawLightSources( Camera* aCam, ID3D11Device * aDevice, ID3D11DeviceContext * aContext );

        /// <summary>
        /// Draw the colliders that may be on any of the objects
        /// </summary>
        /// <param name="aCam"></param>
        /// <param name="aDevice"></param>
        /// <param name="aContext"></param>
        void DrawColliders( Camera* aCam, ID3D11Device * aDevice, ID3D11DeviceContext * aContext );

        /// <summary>
        /// Draw the editor UI
        /// </summary>
        void DrawUI();

        /// <summary>
        /// Draw the hierarchy of objects in the scene
        /// </summary>
        FORCE_INLINE void DrawHierarchy();

        /// <summary>
        /// Draw the current stats of the scene
        /// </summary>
        FORCE_INLINE void DrawStats();

        /// <summary>
        /// Draw the Inspector elements of the currently selected entity
        /// </summary>
        FORCE_INLINE void DrawInspector();

        /// <summary>
        /// Draw any gizmos that should be on screen
        /// </summary>
        void DrawGizmos( ID3D11Device * aDevice, ID3D11DeviceContext * aContext );

        /// <summary>
        /// Behavior that should happen when the scene is reset
        /// </summary>
        void ResetScene();

        /** Static instance of the editor */
        static EditorCore* Instance;

        /** Resource manager for handling any editor-specific shaders */
        ResourceManager* resourceMan = nullptr;

        /** The scene manager of this game */
        SceneManagement::SceneManager* sceneMan = nullptr;

        /** The entity component manager */
        ECS::ComponentManager* compMan = nullptr;

        /** The mesh to use to draw a point light */
        Mesh* PointLightMesh = nullptr;

        /** The outline shader that can be used for the selected object */
        SimplePixelShader* OutlineShader = nullptr;

        /** The unlit shader that we can use to draw light sources */
        SimplePixelShader* UnlitShader = nullptr;

        /** Vertex shader to use for drawing any gizmos */
        SimpleVertexShader* VertexShader = nullptr;

        /** Wireframe state for drawing gizmos */
        ID3D11RasterizerState* WireFrame = nullptr;

        /** Flag to set lighting gizmos */
        UINT32 DoGizmoDraw : 1;

        /** Flag to draw the skybox */
        UINT32 DrawSkyBox : 1;

        /** Flag to draw colliders or not */
        UINT32 DebugDrawColliders : 1;

        UINT32 DebugDrawLights : 1;

        /** The entity that the user wants to change */
        Entity* SelectedEntity = nullptr;

        /** The current scene file to save all entities to */
        std::string SceneFile = "Assets/Scenes/Scene_test.json";

        /** The char buffer for changing the scene name with IMGUI */
        char editSceneNameBuf [ 64 ] = "\0";

        /** The color that will be draw around the entity that's selected */
        glm::vec3 SelectedOutlineColor = { 1.0f, 0.0f, 0.0f };

        /** The current gizmos that should be drawn */
        std::vector<Gizmo*> CurrentGizmos;

    };



}   // namespace Editor