#include "../stdafx.h"

#include "MeshRenderer.h"
#include "../Entity/Entity.h"

#define MAT_SAVE_KEY    "Material"
#define MESH_SAVE_KEY   "Mesh"

COMPONENT_INIT( MeshRenderer )

MeshRenderer::MeshRenderer()
{
    ResourceManager* resMan = ResourceManager::GetInstance();
}

MeshRenderer::MeshRenderer( Material * aMat, Mesh * aMesh )
    : MeshRenderer()
{
    CurrentMaterial = aMat;
    CurrentMesh = aMesh;
}

MeshRenderer::MeshRenderer( nlohmann::json const & aInitData )
{
    ResourceManager* resMan = ResourceManager::GetInstance();

    // Load mesh
    std::string mshFile = aInitData [ MESH_SAVE_KEY ];
    FileName MeshWsTmp( mshFile.begin(), mshFile.end() );
    CurrentMesh = resMan->LoadMesh( MeshWsTmp );

    // Load Material
    std::string matFileName = aInitData [ MAT_FILE_SAVE_KEY ];
    FileName MatWsTemp( matFileName.begin(), matFileName.end() );
    CurrentMaterial = resMan->LoadMaterial( MatWsTemp );
}

MeshRenderer::~MeshRenderer()
{
    CurrentMaterial = nullptr;
    CurrentMesh = nullptr;
}

void MeshRenderer::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    
    // Display the mesh name
    std::string meshName = "No Mesh";

    if ( CurrentMesh != nullptr )
    {
        meshName = CurrentMesh->GetMeshFileName();
    }
    ImGui::Text( "Mesh:\t %s", meshName.c_str() );

    std::string matName = "No Material";
    if ( CurrentMaterial != nullptr )
    {
        matName = CurrentMaterial->GetMeshFileName();
    }
    ImGui::Text( "Material:\t %s", matName.c_str() );

    // Add a button for setting one



}

void MeshRenderer::SaveComponentData( nlohmann::json & aOutFile )
{
    // Save mesh
    if ( CurrentMesh != nullptr )
    {
        aOutFile [ MESH_SAVE_KEY ] = CurrentMesh->GetMeshFileName();
    }
    else
    {
        aOutFile [ MESH_SAVE_KEY ] = "NULL_MESH";
    }
    // Save material
    if ( CurrentMaterial != nullptr )
    {
        aOutFile [ MAT_FILE_SAVE_KEY ] = CurrentMaterial->GetMeshFileName();
    }
    else
    {
        aOutFile [ MAT_FILE_SAVE_KEY ] = "NULL_MAT";
    }
}

void MeshRenderer::PrepareMaterial( const glm::highp_mat4 & aView, const glm::highp_mat4 & aProjection )
{
    assert( CurrentMaterial != nullptr );

    // Render all meshes that are a part of this entity
    // in the future I want to experiment with different meshes/material 
    // settings
    CurrentMaterial->SetShaderValues(
        OwningEntity->GetTransform()->GetWorldMatrix(),
        aView,
        aProjection
    );
}