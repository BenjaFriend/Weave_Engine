#pragma once

#include "../ECS/Component.h"
#include "Mesh.h"
#include "Materials/Material.h"
#include "../MathHelper.h"
#include "../Entity/Transform.h"
#include "../Entity/EntityManager.h"

/// <summary>
/// A mesh renderer is a component that when added to an entity
/// will add it to the render list with the given material
/// </summary>
/// <author>Ben Hoffman</author>
class MeshRenderer : public ECS::Component<MeshRenderer>
{
public:

    COMP_NAME( "MeshRenderer" );

    MeshRenderer( Material* aMat, Mesh* aMesh );

    ~MeshRenderer();

    virtual void DrawEditorGUI() override;

    /// <summary>
    /// Sets the material's shader data and activates the shaders
    /// </summary>
    /// <param name="aView">View matrix</param>
    /// <param name="aProjection">Project matrix</param>
    void PrepareMaterial( const glm::highp_mat4 & aView, const glm::highp_mat4 & aProjection );

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

    inline Material* GetMaterial() const { return CurrentMaterial; }
    inline void SetMaterial( Material* aMat ) { CurrentMaterial = aMat; }

    inline Mesh* GetMesh() const { return CurrentMesh; }
    inline void SetMesh( Mesh* aMesh ) { CurrentMesh = aMesh; }

protected:

    virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

private:

    /** The material to use for this mesh renderer */
    Material* CurrentMaterial = nullptr;

    /** The current mesh to use for this mesh renderer */
    Mesh* CurrentMesh = nullptr;

    /** The transform of this owning entity */
    Transform* ParentTransform = nullptr;

};