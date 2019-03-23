#pragma once

#include "../ECS/Component.h"
#include "Mesh.h"
#include "Materials/Material.h"
#include "MathHelper.h"
#include "../Entity/Transform.h"
#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"
#include "../Resources/ResourceManager.h"

/// <summary>
/// A mesh renderer is a component that when added to an entity
/// will add it to the render list with the given material
/// </summary>
/// <author>Ben Hoffman</author>
class MeshRenderer : public ECS::Component<MeshRenderer>
{
public:

    COMPONENT( MeshRenderer );

    MeshRenderer();

    /// <summary>
    /// Initalize this mesh renderer to the given material and mesh
    /// </summary>
    /// <param name="aMat">The material to use for this mesh renderer</param>
    /// <param name="aMesh">The material to use for this mesh renderer</param>
    MeshRenderer( Material* aMat, Mesh* aMesh );

    /// <summary>
    /// Initalize this mesh renderer from save file data
    /// </summary>
    /// <param name="aInitData">JSON save file data.</param>
    MeshRenderer( nlohmann::json const & aInitData );

    ~MeshRenderer();

    /// <summary>
    /// Drawing of this component in the ECS editor
    /// </summary>
    virtual void DrawEditorGUI() override;

    /// <summary>
    /// Sets the material's shader data and activates the shaders
    /// </summary>
    /// <param name="aView">View matrix</param>
    /// <param name="aProjection">Project matrix</param>
    void PrepareMaterial( const glm::highp_mat4 & aView, const glm::highp_mat4 & aProjection );

    /// <summary>
    /// Attempt to set this renderer's mesh to the given file name. 
    /// If the loading of the mesh failed, do nothing. 
    /// </summary>
    /// <returns>True if success, false if failure</returns>
    bool SetMeshFromFile( const FileName & aFileName );

    /// <summary>
    /// Attempt to set this renderer's material to the given file name.
    /// If the loading of the mesh failed, do nothing
    /// </summary>
    /// <param name="aFileName">The material file to load in '*.wmat'</param>
    /// <returns>True if successful</returns>
    bool SetMaterialFromFile( const FileName & aFileName );

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

    FORCE_INLINE Material* GetMaterial() const { return CurrentMaterial; }
    FORCE_INLINE void SetMaterial( Material* aMat ) { CurrentMaterial = aMat; }

    FORCE_INLINE Mesh* GetMesh() const { return CurrentMesh; }
    FORCE_INLINE void SetMesh( Mesh* aMesh ) { CurrentMesh = aMesh; }


protected:

    virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

private:

    /** The material to use for this mesh renderer */
    Material* CurrentMaterial = nullptr;

    /** The current mesh to use for this mesh renderer */
    Mesh* CurrentMesh = nullptr;

    /** The transform of this owning entity */
    Transform* ParentTransform = nullptr;

    /** The resource manager for changing meshes at runtime */
    ResourceManager* resMan = nullptr;

};