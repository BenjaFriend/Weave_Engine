#include "../stdafx.h"

#include "MeshRenderer.h"

#define MAT_SAVE_KEY    "Material"
#define MESH_SAVE_KEY   "Mesh"

COMPONENT_INIT( MeshRenderer )

MeshRenderer::MeshRenderer( Material * aMat, Mesh * aMesh )
    : CurrentMaterial( aMat ), CurrentMesh( aMesh )
{
}

MeshRenderer::MeshRenderer( nlohmann::json const & aInitData )
{
    LOG_WARN( "Mesh renderer load from scene not yet implemented!" );
}

MeshRenderer::~MeshRenderer()
{
    CurrentMaterial = nullptr;
    CurrentMesh = nullptr;
}

void MeshRenderer::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    // #TODO Have a material / mesh format that we can edit during runtime
    ImGui::Text( "Material:\t %d", ( CurrentMaterial != nullptr ) );
    ImGui::Text( "Mesh:\t %d", ( CurrentMesh != nullptr ) );
}

void MeshRenderer::SaveComponentData( nlohmann::json & aOutFile )
{

}

void MeshRenderer::PrepareMaterial( const glm::highp_mat4 & aView, const glm::highp_mat4 & aProjection )
{
    assert( CurrentMaterial != nullptr );

    // Render all meshes that are a part of this entity
    // in the future I want to experiment with different meshes/material 
    // settings
    if( ParentTransform == nullptr )
        ParentTransform = EntityManager::GetInstance()->GetEntity( owner )->GetTransform();

    CurrentMaterial->SetShaderValues( 
        ParentTransform->GetWorldMatrix(), 
        aView,
        aProjection 
    );
}