#include "../stdafx.h"

#include "MeshRenderer.h"

MeshRenderer::MeshRenderer( Material * aMat, Mesh * aMesh )
    : CurrentMaterial( aMat ), CurrentMesh( aMesh )
{
}

MeshRenderer::~MeshRenderer()
{
    CurrentMaterial = nullptr;
    CurrentMesh = nullptr;
}

void MeshRenderer::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
}

void MeshRenderer::SaveObject( nlohmann::json & aOutFile )
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