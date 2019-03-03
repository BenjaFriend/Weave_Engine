#include "../stdafx.h"

#include "Game.h"
#include "../Resources/Vertex.h"
#include "../Resources/Mesh.h"
#include "../Resources/MeshRenderer.h"
#include "../Entity/Entity.h"
#include "../Entity/Camera.h"
#include "../Resources/Materials/Material.h"

#include "../ECS/IComponent.h"
#include "../Lighting/PointLight.h"
#include "../Lighting/DirLight.h"
#include "../Physics/BoxCollider.h"
#include "../Physics/RigidBody.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game( HINSTANCE hInstance )
    : DXCore(
        hInstance,		   // The application's handle
        "DirectX Game",	   // Text for the window's title bar
        1280,			   // Width of the window's client area
        720,			   // Height of the window's client area
        true )			   // Show extra stats (fps) in title bar?
{

    vertexShader = 0;
    pixelShader = 0;

#if defined( DEBUG ) || defined( _DEBUG )
    // Do we want a console window?  Probably only in debug mode
    CreateConsoleWindow( 500, 120, 32, 120 );
    printf( "Console window created successfully.  Feel free to printf() here.\n" );
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
    skyRastState->Release();
    skyDepthState->Release();
    FlyingCamera = nullptr;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
    FlyingCamera = CameraMan->GetActiveCamera();
    assert( FlyingCamera != nullptr );

#if defined( EDITOR_ON )

    editor = Editor::EditorCore::GetInstance();
    editor->SetCamera( FlyingCamera );
    LOG_TRACE( "Editor Initalized!" );

#endif  // EDITOR_ON

    // Create a wireframe rasterizer state
    D3D11_RASTERIZER_DESC wireRS = {};
    wireRS.FillMode = D3D11_FILL_WIREFRAME;
    wireRS.CullMode = D3D11_CULL_NONE;
    //device->CreateRasterizerState( &wireRS, &WireFrame );
    WireFrame = resourceMan->LoadRasterizerState( "Wireframe", wireRS );

    // Rasterizer state for drawing the inside of my sky box geometry
    D3D11_RASTERIZER_DESC rs = {};
    rs.FillMode = D3D11_FILL_SOLID;
    rs.CullMode = D3D11_CULL_FRONT;
    rs.DepthClipEnable = true;
    device->CreateRasterizerState( &rs, &skyRastState );

    D3D11_DEPTH_STENCIL_DESC ds = {};
    ds.DepthEnable = true;
    ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    device->CreateDepthStencilState( &ds, &skyDepthState );

    // Helper methods for loading shaders, creating some basic
    // geometry to draw and some simple camera matrices.
    //  - You'll be expanding and/or replacing these later
    LoadShaders();
    CreateMatrices();
    CreateBasicGeometry();
    InitLights();

    // Tell the input assembler stage of the pipeline what kind of
    // geometric primitives (points, lines or triangles) we want to draw.  
    // Essentially: "What kind of shape should the GPU draw with our data?"
    context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    inputManager->BindAction( this, &Game::OnMouseDown, Input::InputType::Look );
    inputManager->BindAction( this, &Game::OnMouseUp, Input::InputType::LookReleased );
    inputManager->BindAction( this, &Game::Quit, Input::InputType::Quit );

    ScriptMan->LoadScripts();
    inputManager->BindAction( ScriptMan, &Scripting::ScriptManager::OnClick, Input::InputType::Fire );
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
    vertexShader = resourceMan->LoadShader<SimpleVertexShader>(
        L"VertexShader.cso" );

    pixelShader = resourceMan->LoadShader<SimplePixelShader>(
        L"PixelShader.cso" );

    UnlitPixelShader = resourceMan->LoadShader<SimplePixelShader>(
        L"PixelShader_Unlit.cso" );

    SkyBoxVS = resourceMan->LoadShader<SimpleVertexShader>(
        L"SkyVS.cso" );

    SkyBoxPS = resourceMan->LoadShader<SimplePixelShader>(
        L"SkyPS.cso" );
}

void Game::InitLights()
{
    glm::vec3 Red = glm::vec3( 1.0f, 0.0f, 0.0f );
    glm::vec3 Blue = glm::vec3( 0.0f, 0.0f, 1.0f );
    glm::vec3 White = glm::vec3( 1.0f, 1.0f, 1.0f );

    DirectionalLightData DirLight1 = {};
    DirLight1.AmbientColor = glm::vec4( 1.f, 1.f, 1.f, 1.0f ); // Ambient color is the color when we are in shadow
    DirLight1.DiffuseColor = glm::vec4( 1.f, 1.f, 1.f, 1.0f );
    DirLight1.Direction = glm::vec3( 1.0f, 0.0f, 0.0f );
    DirLight1.Intensity = 1.f;


    // Add Dir Lights
    Entity* dirLightEntity = sceneManager->GetActiveScene()->AddEntity( "Dir Light 1" );
    dirLightEntity->AddComponent<DirLight>( DirLight1 );

    // Add Point Lights
    Entity* pLightEntity = sceneManager->GetActiveScene()->AddEntity( "Point Light 1" );
    pLightEntity->AddComponent<PointLight>( Red, glm::vec3( 0.f, 2.0f, 0.0f ) );

    Entity* pLightEntity2 = sceneManager->GetActiveScene()->AddEntity( "Point Light 2" );
    pLightEntity2->AddComponent<PointLight>( Blue, glm::vec3( 0.f, -1.0f, 0.0f ) );
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
    FlyingCamera->UpdateProjectionMatrix( ( float ) width, ( float ) height );
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
    // Load in the meshes
    PointLightMesh = resourceMan->LoadMesh( L"Assets/Models/sphere.obj" );

    // Create the basic sampler ---------------------------------------------
    D3D11_SAMPLER_DESC samplerDesc = {}; // Zero out the struct memory
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    SamplerID = resourceMan->AddSampler( samplerDesc );

    // Load floor --------------------------------------------------------
    CubeMesh = resourceMan->LoadMesh( L"Assets/Models/cube.obj" );

    // Load in the skybox SRV --------------------------------------------------------
    SkyboxSrvID = resourceMan->LoadSRV_DDS( L"Assets/Textures/SunnyCubeMap.dds" );
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
    // Handle base-level DX resize stuff
    DXCore::OnResize();

    FlyingCamera->UpdateProjectionMatrix( static_cast< float >( width ), static_cast< float >( height ) );
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update( float dt, float totalTime )
{
    inputManager->Update( dt );

    // Update the camera
    FlyingCamera->Update( dt );
    FlyingCamera->UpdateProjectionMatrix( static_cast< float >( width ), static_cast< float >( height ) );
    
    ScriptMan->Update( dt );

#if defined( EDITOR_ON )

    editor->Update( dt );

#endif  // EDITOR_ON
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw( float dt, float totalTime )
{
    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of Draw (before drawing *anything*)
    context->ClearRenderTargetView( backBufferRTV, ( float* ) ( &BackgroundColor ) );
    context->ClearDepthStencilView(
        depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0 );

    assert( FlyingCamera != nullptr );
    
    // Set buffers in the input assembler
    //  - Do this ONCE PER OBJECT you're drawing, since each object might
    //    have different geometry.
    UINT stride = sizeof( Vertex );
    UINT offset = 0;

    const Mesh* EnMesh = nullptr;
    Material* EnMat = nullptr;
    MeshRenderer* MeshRend = nullptr;
    ID3D11Buffer* VertBuff = nullptr;
    ID3D11Buffer* IndexBuf = nullptr;
    Entity* CurrentEntity = nullptr;
    UINT IndexCount = 0;

    SceneManagement::Scene* CurScene = sceneManager->GetActiveScene();

    Entity* entArray = CurScene->GetEntityArray();

    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        CurrentEntity = &entArray [ i ];
        if ( CurrentEntity == nullptr || !CurrentEntity->GetIsActive() || !CurrentEntity->GetIsValid() ) continue;

        MeshRend = CurrentEntity->GetComponent<MeshRenderer>();
        if ( MeshRend != nullptr )
        {
            EnMat = MeshRend->GetMaterial();

            if ( EnMat == nullptr ) continue;

            // Send camera info ---------------------------------------------------------
            EnMat->GetPixelShader()->SetFloat3(
                "CameraPosition",
                FlyingCamera->GetEntity()->GetTransform()->GetPosition()
            );

            MeshRend->PrepareMaterial(
                FlyingCamera->GetViewMatrix(),
                FlyingCamera->GetProjectMatrix()
            );

            // Draw the entity ---------------------------------------------------------
            EnMesh = MeshRend->GetMesh();
            if ( EnMesh == nullptr ) continue;

            IndexBuf = EnMesh->GetIndexBuffer();
            VertBuff = EnMesh->GetVertexBuffer();
            IndexCount = EnMesh->GetIndexCount();

            // Set the buffer information
            context->IASetVertexBuffers( 0, 1, &VertBuff, &stride, &offset );
            context->IASetIndexBuffer( IndexBuf, DXGI_FORMAT_R32_UINT, 0 );

            // Finally do the actual drawing
            context->DrawIndexed( IndexCount, 0, 0 );

            CurScene->SetShaderInfo(
                EnMat->GetVertexShader(),
                EnMat->GetPixelShader()
            );
        }

    }   // end Entity loop

    // Draw the Sky box -------------------------------------

    if ( DrawSkyBox )
    {
        // Set up sky render states
        context->RSSetState( skyRastState );
        context->OMSetDepthStencilState( skyDepthState, 0 );

        // After drawing all of our regular (solid) objects, draw the sky!
        ID3D11Buffer* skyVB = CubeMesh->GetVertexBuffer();
        ID3D11Buffer* skyIB = CubeMesh->GetIndexBuffer();

        // Set the buffers
        context->IASetVertexBuffers( 0, 1, &skyVB, &stride, &offset );
        context->IASetIndexBuffer( skyIB, DXGI_FORMAT_R32_UINT, 0 );

        SkyBoxVS->SetMatrix4x4( "view", FlyingCamera->GetViewMatrix() );
        SkyBoxVS->SetMatrix4x4( "projection", FlyingCamera->GetProjectMatrix() );

        SkyBoxVS->CopyAllBufferData();
        SkyBoxVS->SetShader();

        // Send texture-related stuff
        SkyBoxPS->SetShaderResourceView( "SkyTexture", SkyboxSrvID );
        SkyBoxPS->SetSamplerState( "BasicSampler", resourceMan->GetSampler( SamplerID ) );

        SkyBoxPS->CopyAllBufferData(); // Remember to copy to the GPU!!!!
        SkyBoxPS->SetShader();

        // Draw the skybox "mesh"
        context->DrawIndexed( CubeMesh->GetIndexCount(), 0, 0 );

        // Reset any states we've changed for the next frame!
        context->RSSetState( 0 );
        context->OMSetDepthStencilState( 0, 0 );
    }

    if ( DebugDrawColliders )
        DrawColliders();

    if ( DrawLightGizmos )
        DrawLightSources();

#if defined ( ENABLE_UI )

    // Create a new IMGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Draw any game UI
    DrawUI();

    // Draw the editor if we want to
#if defined( EDITOR_ON )

    editor->Draw( dt, device, context );

#endif  // EDITOR_ON

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

#endif

    // Present the back buffer to the user
    //  - Puts the final frame we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
    swapChain->Present( 0, 0 );
    //swapChain->Present( 1, 0 );   // Present with vsync
}

// #Editor
void Game::DrawLightSources()
{
    Mesh* lightMesh = PointLightMesh;
    ID3D11Buffer * vb = lightMesh->GetVertexBuffer();
    ID3D11Buffer * ib = lightMesh->GetIndexBuffer();
    unsigned int indexCount = lightMesh->GetIndexCount();

    // Turn on these shaders
    vertexShader->SetShader();
    UnlitPixelShader->SetShader();

    // Set up vertex shader
    vertexShader->SetMatrix4x4( "view", FlyingCamera->GetViewMatrix() );
    vertexShader->SetMatrix4x4( "projection", FlyingCamera->GetProjectMatrix() );

    // Set buffers in the input assembler
    UINT stride = sizeof( Vertex );
    UINT offset = 0;

    auto PointLights = SceneManagement::SceneManager::GetInstance()->GetActiveScene()->GetPointLights();

    for ( size_t i = 0; i < PointLights.size(); ++i )
    {
        if ( !PointLights [ i ]->IsEnabled() ) continue;

        PointLightData light = PointLights [ i ]->GetLightData();

        context->IASetVertexBuffers( 0, 1, &vb, &stride, &offset );
        context->IASetIndexBuffer( ib, DXGI_FORMAT_R32_UINT, 0 );

        float scale = 0.5f;

        XMMATRIX rotMat = XMMatrixIdentity();
        XMMATRIX scaleMat = XMMatrixScaling( scale, scale, scale );
        XMMATRIX transMat = XMMatrixTranslation( light.Position.x, light.Position.y, light.Position.z );

        // Make the transform for this light
        XMFLOAT4X4 world;
        XMStoreFloat4x4( &world, XMMatrixTranspose( scaleMat * rotMat * transMat ) );

        // Set up the world matrix for this light
        vertexShader->SetMatrix4x4( "world", world );

        // Set up the pixel shader data
        glm::vec3 finalColor = light.Color;
        finalColor.x *= light.Intensity;
        finalColor.y *= light.Intensity;
        finalColor.z *= light.Intensity;
        UnlitPixelShader->SetFloat3( "Color", finalColor );

        // Copy data
        vertexShader->CopyAllBufferData();
        UnlitPixelShader->CopyAllBufferData();
        context->DrawIndexed( indexCount, 0, 0 );

        // Wireframe mode ---------------------------------
        if ( PointLights [ i ]->GetDrawRange() )
        {
            scaleMat = XMMatrixScaling( light.Range, light.Range, light.Range );

            // Make the transform for this light
            XMStoreFloat4x4( &world, XMMatrixTranspose( scaleMat * rotMat * transMat ) );

            // Draw the wireframe point light range
            vertexShader->SetMatrix4x4( "world", world );

            UnlitPixelShader->SetFloat3( "Color", finalColor );

            // Copy data to the shaders
            vertexShader->CopyAllBufferData();
            UnlitPixelShader->CopyAllBufferData();

            // Set the wireframe rasterizer state
            context->RSSetState( WireFrame );
            // Draw the wireframe
            context->DrawIndexed( indexCount, 0, 0 );

            // Reset the Rasterizer state
            context->RSSetState( 0 );
        }
    }
}

void Game::DrawColliders()
{
    /*Mesh* cubeMesh = CubeMesh;
    ID3D11Buffer * vb = cubeMesh->GetVertexBuffer();
    ID3D11Buffer * ib = cubeMesh->GetIndexBuffer();
    unsigned int indexCount = cubeMesh->GetIndexCount();

    // Turn on these shaders
    vertexShader->SetShader();
    UnlitPixelShader->SetShader();

    // Set up vertex shader
    vertexShader->SetMatrix4x4( "view", FlyingCamera->GetViewMatrix() );
    vertexShader->SetMatrix4x4( "projection", FlyingCamera->GetProjectMatrix() );

    // Set buffers in the input assembler

    auto colliders = PhysicsMan->GetColliders();
    for ( Physics::BoxCollider* box : colliders )
    {
        const glm::vec3 & extents = box->GetExtents();
        const glm::vec3 & pos = box->GetPosition();

        XMMATRIX rotMat = XMMatrixIdentity();
        XMMATRIX scaleMat = XMMatrixScaling( extents.x, extents.y, extents.z );
        XMMATRIX transMat = XMMatrixTranslation( pos.x, pos.y, pos.z );

        // Make the transform for this light
        VEC4x4 world;

        // Wireframe mode ---------------------------------
        // Make the transform for this light
        XMStoreFloat4x4( &world, XMMatrixTranspose( scaleMat * rotMat * transMat ) );

        // Draw the wire frame point light range
        vertexShader->SetMatrix4x4( "world", world );

        UnlitPixelShader->SetFloat3( "Color", glm::vec3( 1.0f, 1.0f, 0.0f ) );

        // Copy data to the shaders
        vertexShader->CopyAllBufferData();
        UnlitPixelShader->CopyAllBufferData();

        // Set the wire frame Rasterizer state
        context->RSSetState( WireFrame );

        // Draw the wire frame
        context->DrawIndexed( indexCount, 0, 0 );
    }

    // Reset the Rasterizer state
    context->RSSetState( 0 );*/
}

void Game::DrawUI()
{

    // Options --------------------------
    {
        ImGui::Begin( "Demo Options" );

        ImGui::Checkbox( "Draw Light Gizmos", &DrawLightGizmos );

        ImGui::Checkbox( "Use SkyBox", &DrawSkyBox );

        ImGui::Separator();

        ImGui::End();   // If you want more than one window, then use ImGui::Beigin
    }
}

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown()
{
    // Add any custom code here...
    FlyingCamera->SetDoRotation( true );

    // Save the previous mouse position, so we have it for the future
    //prevMousePos.x = x;
    //prevMousePos.y = y;

    // Caputure the mouse so we keep getting mouse move
    // events even if the mouse leaves the window.  we'll be
    // releasing the capture once a mouse button is released
    SetCapture( hWnd );
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp()
{
    // Add any custom code here...
    // Reverse the camera direction
    FlyingCamera->SetDoRotation( false );

    // We don't care about the tracking the cursor outside
    // the window anymore (we're not dragging if the mouse is up)
    ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove( WPARAM buttonState, int x, int y )
{
    FlyingCamera->UpdateMouseInput( prevMousePos.x - x, prevMousePos.y - y );

    // Save the previous mouse position, so we have it for the future
    prevMousePos.x = x;
    prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel( float wheelDelta, int x, int y )
{
    // Add any custom code here...
}

void Game::Quit()
{
    DXCore::Quit();
}

#pragma endregion