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

void EditorCore::SetDrawGizmos( const bool aDrawGizmos )
{
    DoGizmoDraw = aDrawGizmos;
}

void Editor::EditorCore::SetSceneFile( const FileName & aFileName )
{
    SceneFile = aFileName;
}

EditorCore::EditorCore()
{
    sceneMan = SceneManagement::SceneManager::GetInstance();
    sceneMan->OnSceneUnload().BindListener( this, &Editor::EditorCore::ResetScene );

    LoadResources();
}

EditorCore::~EditorCore()
{
    SelectedEntity = nullptr;
    sceneMan = nullptr;

    // Delete all gizmos
    for ( size_t i = 0; i < CurrentGizmos.size(); ++i )
    {
        if ( CurrentGizmos [ i ] != nullptr )
            delete CurrentGizmos [ i ];
    }
    CurrentGizmos.clear();
}

void EditorCore::LoadResources()
{
    resourceMan = ResourceManager::GetInstance();
    OutlineShader = resourceMan->LoadShader<SimplePixelShader>(
        L"PixelShader_Unlit.cso" );

    VertexShader = resourceMan->LoadShader<SimpleVertexShader>(
        L"VertexShader.cso" );

    UnlitShader = resourceMan->LoadShader<SimplePixelShader>(
        L"PixelShader_Unlit.cso" );

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

    if ( DoGizmoDraw )
        DrawGizmos( aDevice, aContext );

    // Draw the selected object
    if ( SelectedEntity != nullptr )
    {
        MeshRenderer* MeshRend = SelectedEntity->GetComponent<MeshRenderer>();
        if ( MeshRend == nullptr )
        {
            return;
        }
        const Mesh* mesh = MeshRend->GetMesh();
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
        glm::mat4x4 world = SelectedEntity->GetTransform()->GetWorldMatrix();

        // Set up vertex shader
        VertexShader->SetMatrix4x4( "world", world );
        VertexShader->SetMatrix4x4( "view", CurrentCamera->GetViewMatrix() );
        VertexShader->SetMatrix4x4( "projection", CurrentCamera->GetProjectMatrix() );

        VertexShader->SetShader();
        VertexShader->CopyAllBufferData();

        // Setup the outline shader
        OutlineShader->SetFloat3( "Color", SelectedOutlineColor );

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

    ImGuiWindowFlags corner =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoTitleBar;
    // Draw the file menu ----------------------
    {
        bool isOpen = true;
        ImGui::Begin( "File Options", &isOpen, corner );
        ImGui::SetWindowPos( ImVec2( 0, 0 ), true );

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "File" ) )
            {
                if ( ImGui::MenuItem( "Open Scene", "Ctrl+O" ) )
                {
                    LOG_TRACE( "Open file!" );
                    LoadScene();
                    // #TODO Set up a load scene file using the scene manager
                }

                if ( ImGui::MenuItem( "Save Scene ...", "Ctrl+S" ) )
                {
                    LOG_TRACE( "Save file!" );
                    SaveScene();
                }

                if ( ImGui::MenuItem( "Unload Scene" ) )
                {
                    LOG_TRACE( "Unload Scene!" );
                    SceneManagement::SceneManager::GetInstance()->UnloadCurrentScene();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    // Stats and Info ---------------------------
    DrawStats();

    // Draw the hierarchy of objects --------------------------
    DrawHierarchy();

    // Inspector --------------------------
    DrawInspector();

#endif
}

inline void EditorCore::DrawHierarchy()
{
    ImGui::Begin( "Hierarchy" );

    Entity* CurrentEntity = nullptr;
    SceneManagement::Scene* CurScene = sceneMan->GetActiveScene();
    const std::vector<Entity*> & entArray = CurScene->GetEntityArray();
    for ( size_t i = 0; i < entArray.size(); ++i )
    {
        CurrentEntity = entArray [ i ];
        if ( CurrentEntity == nullptr ) continue;
        if ( ImGui::Button( CurrentEntity->GetName().c_str(), ImVec2( ImGui::GetWindowWidth(), 0.f ) ) )
        {
            SelectedEntity = CurrentEntity;
        }
        ImGui::Separator();
    }

    ImGui::End();
}

FORCE_INLINE void Editor::EditorCore::DrawStats()
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

FORCE_INLINE void Editor::EditorCore::DrawInspector()
{
    if ( SelectedEntity == nullptr ) return;

    ImGui::Begin( "Inspector" );

    bool isActive = SelectedEntity->GetIsActive();
    ImGui::Checkbox( "Active", &isActive ); ImGui::SameLine();

    char newNameBuf [ 256 ];
    strcpy_s( newNameBuf, SelectedEntity->GetName().c_str() );
    ImGui::InputText( "Name", newNameBuf, 256 );

    if ( ImGui::Button( "Delete" ) )
    {
        SceneManagement::SceneManager::GetInstance()->GetActiveScene()->DeleteEntity( SelectedEntity );
        SelectedEntity = nullptr;
        ImGui::End();
        return;
    }

    SelectedEntity->SetName( newNameBuf );
    SelectedEntity->SetIsActive( isActive );

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

void EditorCore::DrawGizmos( ID3D11Device * aDevice, ID3D11DeviceContext * aContext )
{
    for ( auto itr = CurrentGizmos.begin(); itr != CurrentGizmos.end(); ++itr )
    {

    }
}

void Editor::EditorCore::ResetScene()
{
    SelectedEntity = nullptr;
}

void EditorCore::SaveScene()
{
    LOG_TRACE( "Save scene!" );
    nlohmann::json njson;

    njson [ SCENE_NAME_SAVE_KEY ] = "Test_Scene_Name";
    njson [ ENTITY_ARRAY_SAVE_KEY ] = nlohmann::json::array();

    Entity* CurrentEntity = nullptr;

    SceneManagement::Scene* CurScene = sceneMan->GetActiveScene();
    const std::vector<Entity*> & entArray = CurScene->GetEntityArray();

    for ( size_t i = 0; i < entArray.size(); ++i )
    {
        CurrentEntity = entArray [ i ];
        if ( CurrentEntity != nullptr )
        {
            CurrentEntity->SaveObject( njson [ ENTITY_ARRAY_SAVE_KEY ] );
        }
    }

    // Open a file stream to the given scene file
    std::ofstream ofs( SceneFile );
    if ( ofs.is_open() )
    {
        ofs << std::setw( 4 ) << njson << std::endl;
    }
    else
    {
        LOG_ERROR( "Failed to save scene!" );
    }
    ofs.close();
}

void EditorCore::LoadScene()
{
    LOG_TRACE( "Editor: Load scene!" );
    SceneManagement::SceneManager::GetInstance()->LoadScene( SceneFile );
}