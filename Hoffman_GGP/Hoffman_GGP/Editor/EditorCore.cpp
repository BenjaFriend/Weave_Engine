#include "../stdafx.h"

#include "EditorCore.h"

using namespace Editor;

// Initialize the editor singleton
EditorCore* EditorCore::Instance = nullptr;

EditorCore * EditorCore::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new EditorCore();
    }
    return Instance;
}

void EditorCore::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
    }
}

void EditorCore::SetDrawLightGizmos( const bool aDrawGizmos )
{
    DrawLightGizmos = aDrawGizmos; 
}

void Editor::EditorCore::SetSceneFile( const FileName & aFileName )
{
    SceneFile = aFileName;
}

EditorCore::EditorCore()
{
    entityMan = EntityManager::GetInstance();

    LoadResources();
}

EditorCore::~EditorCore()
{
    entityMan = nullptr;
    SelectedEntity = nullptr;
}

void EditorCore::LoadResources()
{
    resourceMan = ResourceManager::GetInstance();
    OutlineShader = resourceMan->LoadShader<SimplePixelShader>(
        L"PixelShader_Unlit.cso" );

    VertexShader = resourceMan->LoadShader<SimpleVertexShader>(
        L"VertexShader.cso" );

    D3D11_RASTERIZER_DESC wireRS = {};
    wireRS.FillMode = D3D11_FILL_WIREFRAME;
    wireRS.CullMode = D3D11_CULL_NONE;
    WireFrame = resourceMan->LoadRasterizerState( "Wireframe", wireRS );
}

void EditorCore::Update( float dt )
{

}

void EditorCore::Draw( float dt, ID3D11Device * aDevice, ID3D11DeviceContext * aContext )
{
    assert( CurrentCamera != nullptr && VertexShader != nullptr && OutlineShader != nullptr );
    DrawUI();

    DrawGizmos();

    // Draw the selected object
    if ( SelectedEntity != nullptr )
    {
        Mesh* mesh = SelectedEntity->GetEntityMesh();
        // If there is no mesh, default to a sphere
        if ( mesh == nullptr )
        {
            mesh = resourceMan->GetMesh( L"Assets/Models/sphere.obj" );
        }

        ID3D11Buffer * vb = mesh->GetVertexBuffer();
        ID3D11Buffer * ib = mesh->GetIndexBuffer();
        unsigned int indexCount = mesh->GetIndexCount();

        // Set buffers in the input assembler
        UINT stride = sizeof( Vertex );
        UINT offset = 0;

        // Make the transform for this light
        VEC4x4 world = SelectedEntity->GetWorldMatrix();

        // Set up vertex shader
        VertexShader->SetMatrix4x4( "world", world );
        VertexShader->SetMatrix4x4( "view", CurrentCamera->GetViewMatrix() );
        VertexShader->SetMatrix4x4( "projection", CurrentCamera->GetProjectMatrix() );

        VertexShader->SetShader();
        VertexShader->CopyAllBufferData();

        // Setup the outline shader
        OutlineShader->SetFloat3( "Color", VEC3( 1.0f, 0.0f, 0.0f ) );

        OutlineShader->SetShader();
        OutlineShader->CopyAllBufferData();


        aContext->RSSetState( WireFrame );

        aContext->DrawIndexed( indexCount, 0, 0 );

        aContext->RSSetState( 0 );
    }
}

void EditorCore::DrawUI()
{
#if defined( ENABLE_UI )
    // Stats and Info ---------------------------
    {   
        ImGui::Begin( "Info" );
        ImGui::Text( "%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate );

        ImGui::Text( "%.1f FPS", ImGui::GetIO().Framerate );
        ImGui::Separator();

        ImGui::Text( "R.Click - Rotate" );
        ImGui::Text( "WASD    - Move" );
        ImGui::Text( "Space   - Go Up" );
        ImGui::Text( "X       - Go Down" );

        ImGui::End();
    }

    // Draw the hierarchy of objects --------------------------
    {   
        ImGui::Begin( "Hierarchy" );

        Entity* CurrentEntity = nullptr;

        for ( size_t i = 0; i < entityMan->GetEntityCount(); ++i )
        {
            CurrentEntity = entityMan->GetEntity( i );

            if ( ImGui::Button( CurrentEntity->GetName().c_str(), ImVec2( ImGui::GetWindowWidth(), 0.f ) ) )
            {
                SelectedEntity = CurrentEntity;
            }
            ImGui::Separator();
        }

        ImGui::End();
    }

    // Inspector --------------------------
    {   
        if ( SelectedEntity != nullptr )
        {
            ImGui::Begin( "Inspector" );

            bool isActive = SelectedEntity->GetIsActive();
            ImGui::Checkbox( "Active", &isActive ); ImGui::SameLine();

            char newNameBuf [ 256 ];
            strcpy_s( newNameBuf, SelectedEntity->GetName().c_str() );
            ImGui::InputText( "Name", newNameBuf, 256 );

            SelectedEntity->SetName( newNameBuf );
            SelectedEntity->SetIsActive( isActive );

            ImGui::Separator();

            if ( ImGui::CollapsingHeader( "Transform" ) )
            {
                DirectX::XMFLOAT3 newPos = SelectedEntity->GetPosition();
                ImGui::InputFloat3( "Position", ( float* ) &newPos );

                DirectX::XMFLOAT3 newScale = SelectedEntity->GetScale();
                ImGui::InputFloat3( "Scale", ( float* ) &newScale );

                DirectX::XMFLOAT4 newRotation = SelectedEntity->GetRotation();
                ImGui::InputFloat4( "Rotation", ( float* ) &newRotation );

                // The position of the current object
                SelectedEntity->SetPosition( newPos );
                SelectedEntity->SetScale( newScale );
                SelectedEntity->SetRotation( newRotation );
            }

            ImGui::Separator();

            // Loop through each of this entity's components
            auto compMap = SelectedEntity->GetAllComponents();
            if ( compMap != nullptr )
            {
                for ( auto compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
                {
                    ImGui::Separator();

                    ECS::IComponent* theComp = ( compItr->second );
                    if ( theComp != nullptr )
                    {
                        if ( ImGui::CollapsingHeader( theComp->ComponentName() ) )
                        {
                            theComp->DrawEditorGUI();
                        }
                    }
                }
            }

            ImGui::End();
        }
    }

#endif
}

void EditorCore::DrawGizmos()
{
}

void EditorCore::SaveScene()
{
    nlohmann::json njson;

    njson [ "Scene Name" ] = "Test_Scene";

    Entity* CurrentEntity = entityMan->GetEntity( 0 );

    for ( size_t i = 0; i < entityMan->GetEntityCount(); ++i )
    {
        CurrentEntity = entityMan->GetEntity( i );

        CurrentEntity->SaveObject( njson );
    }

    std::ofstream ofs( SceneFile );
    if ( ofs.is_open() )
    {
        ofs << std::setw( 4 ) << njson << std::endl;
    }
    else
    {
        //LOG_ERROR( "Failed to save scene: {}", SceneFile );
    }
    ofs.close();
}

void EditorCore::LoadScene()
{
    std::ifstream ifs( SceneFile );
    if ( ifs.is_open() )
    {
        // Store the info in the scene file in the JSON object
        nlohmann::json njson;
        ifs >> njson;
        nlohmann::json::iterator it = njson [ "Entities" ].begin();

        for ( ; it != njson [ "Entities" ].end(); ++it )
        {
            // Key is the name 
            LOG_TRACE( "Entity: {}\n", it.key() );

            // Create a new entity

            // Value is all the components
            nlohmann::json::iterator compItr = njson [ "Entities" ] [ it.key() ].begin();
            for ( ; compItr != njson [ "Entities" ] [ it.key() ].end(); ++compItr )
            {
                std::cout << "Comp: " << compItr.key() << " :: " << compItr.value() << "\n";
                // Add component of this type

            }
        }
    }
    else
    {
        //LOG_ERROR( "Failed to load scene: {}", SceneFile );
    }

    ifs.close();
}