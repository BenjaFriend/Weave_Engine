#include "Game.h"
#include "../Resources/Vertex.h"
#include "../Resources/Mesh.h"
#include "../Entity/Entity.h"
#include "../Entity/Camera.h"
#include "../Resources/Materials/Material.h"

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
    //EntityCount = 0;
    FlyingCamera = new Camera();

    //#if defined(DEBUG) || defined(_DEBUG)
        // Do we want a console window?  Probably only in debug mode
    CreateConsoleWindow( 500, 120, 32, 120 );
    printf( "Console window created successfully.  Feel free to printf() here.\n" );
    //#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
    delete vertexShader;
    delete pixelShader;
    delete UnlitPixelShader;
    delete SkyBoxVS;
    delete SkyBoxPS;

    skyRastState->Release();
    skyDepthState->Release();

    EntityManager::ReleaseInstance();

    ResourceManager::ReleaseInstance();

    delete FlyingCamera;

    Input::InputManager::Release();

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
    entityMan = EntityManager::GetInstance();
    resourceMan = ResourceManager::Initalize( device );

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

    inputManager->BindAction( this, &Game::OnLookDown, Input::InputType::Look );
    inputManager->BindAction( this, &Game::OnLookUp, Input::InputType::LookReleased );

    BackgroundColor = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
    vertexShader = new SimpleVertexShader( device, context );
    vertexShader->LoadShaderFile( L"VertexShader.cso" );

    pixelShader = new SimplePixelShader( device, context );
    pixelShader->LoadShaderFile( L"PixelShader.cso" );

    UnlitPixelShader = new SimplePixelShader( device, context );
    UnlitPixelShader->LoadShaderFile( L"PixelShader_Unlit.cso" );

    SkyBoxVS = new SimpleVertexShader( device, context );
    SkyBoxVS->LoadShaderFile( L"SkyVS.cso" );

    SkyBoxPS = new SimplePixelShader( device, context );
    SkyBoxPS->LoadShaderFile( L"SkyPS.cso" );
}

void Game::InitLights()
{
    DirectionalLight Light1 = {};
    Light1.AmbientColor = XMFLOAT4( 1.f, 1.f, 1.f, 1.0f ); // Ambient color is the color when we are in shadow
    Light1.DiffuseColor = XMFLOAT4( 1.f, 1.f, 1.f, 1.0f );
    Light1.Direction = XMFLOAT3( 1.0f, 0.0f, 0.0f );
    Light1.Intensity = 0.1f;
    DirLights.emplace_back( Light1 );

    DirectionalLight Light2 = {};
    Light2.AmbientColor = XMFLOAT4( 1.f, 1.f, 1.f, 1.0f );
    Light2.DiffuseColor = XMFLOAT4( 0.5f, 1.f, 1.f, 0.1f );
    Light2.Direction = XMFLOAT3( -1.0f, 0.0f, 0.5f );
    Light2.Intensity = 1.0f;
    DirLights.emplace_back( Light2 );

    XMFLOAT3 Red = XMFLOAT3( 1.0f, 0.0f, 0.0f );
    XMFLOAT3 Blue = XMFLOAT3( 0.0f, 0.0f, 1.0f );
    XMFLOAT3 White = XMFLOAT3( 1.0f, 1.0f, 1.0f );

    PointLight pLight1 = {};
    pLight1.Color = Red;
    pLight1.Position = XMFLOAT3( 0.f, 2.0f, 0.0f );
    pLight1.Intensity = 2.f;
    pLight1.Range = 5.f;
    PointLights.emplace_back( pLight1 );

    PointLight pLight2 = {};
    pLight2.Color = Blue;
    pLight2.Position = XMFLOAT3( 0.f, -1.0f, 0.0f );
    pLight2.Intensity = 5.f;
    pLight2.Range = 2.f;
    PointLights.emplace_back( pLight2 );
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
    FlyingCamera->UpdateProjectionMatrix( width, height );
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
    // Load in the meshes
    PointLightMesh_ID = resourceMan->LoadMesh( "Assets/Models/sphere.obj" );
    size_t meshID = PointLightMesh_ID;

    // Create the basic sampler ---------------------------------------------
    D3D11_SAMPLER_DESC samplerDesc = {}; // Zero out the struct memory
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    SamplerID = resourceMan->AddSampler( samplerDesc );

    // Create the stone sphere --------------------------------------------------
    SRV_ID diffSRV = resourceMan->LoadSRV( context, L"Assets/Textures/cobblestone_albedo.png" );
    SRV_ID normSRV = resourceMan->LoadSRV( context, L"Assets/Textures/cobblestone_normals.png" );
    SRV_ID roughnessMap = resourceMan->LoadSRV( context, L"Assets/Textures/cobblestone_roughness.png" );
    SRV_ID metalMap = resourceMan->LoadSRV( context, L"Assets/Textures/cobblestone_metal.png" );

    Material_ID matID = resourceMan->LoadMaterial( vertexShader, pixelShader, diffSRV, normSRV, roughnessMap, metalMap, SamplerID );

    entityMan->AddEntity(
        resourceMan->GetMesh( meshID ), resourceMan->GetMaterial( matID ), XMFLOAT3( 1.f, 0.f, 0.f ) );

    // Load Wood ball --------------------------------------------------------
    SRV_ID woodDif = resourceMan->LoadSRV( context, L"Assets/Textures/wood_albedo.png" );
    SRV_ID woodNormSRV = resourceMan->LoadSRV( context, L"Assets/Textures/wood_normals.png" );
    SRV_ID woodRoughnessMap = resourceMan->LoadSRV( context, L"Assets/Textures/wood_roughness.png" );
    SRV_ID woodMetalMap = resourceMan->LoadSRV( context, L"Assets/Textures/wood_metal.png" );
    Material_ID woodMatID = resourceMan->LoadMaterial( vertexShader, pixelShader, woodDif, woodNormSRV, woodRoughnessMap, woodMetalMap, SamplerID );

    XMFLOAT3 newPos = XMFLOAT3( -1.f, 0.f, 0.f );
    Entity_ID woodEntID = entityMan->AddEntity(
        resourceMan->GetMesh( meshID ), resourceMan->GetMaterial( woodMatID ), newPos );
    entityMan->GetEntity( woodEntID )->SetMass( 0.5f );

    // load Bronze ball --------------------------------------------------------
    SRV_ID bronzeDif = resourceMan->LoadSRV( context, L"Assets/Textures/bronze_albedo.png" );
    SRV_ID bronzeNormSRV = resourceMan->LoadSRV( context, L"Assets/Textures/bronze_normals.png" );
    SRV_ID bronzeRoughnessMap = resourceMan->LoadSRV( context, L"Assets/Textures/bronze_roughness.png" );
    SRV_ID bronzeMetalMap = resourceMan->LoadSRV( context, L"Assets/Textures/bronze_metal.png" );
    Material_ID bronzeMatID = resourceMan->LoadMaterial( vertexShader, pixelShader, bronzeDif, bronzeNormSRV, bronzeRoughnessMap, bronzeMetalMap, SamplerID );

    Entity_ID bronzeEntID = entityMan->AddEntity(
        resourceMan->GetMesh( meshID ), resourceMan->GetMaterial( bronzeMatID ), XMFLOAT3( -2.f, 0.f, 0.f ) );
    entityMan->GetEntity( bronzeEntID )->SetMass( 10.f );
    SelectedEntity = entityMan->GetEntity( bronzeEntID );
    
    // Load floor --------------------------------------------------------
    CubeMeshID = resourceMan->LoadMesh( "Assets/Models/cube.obj" );
    SRV_ID floorDif = resourceMan->LoadSRV( context, L"Assets/Textures/floor_albedo.png" );
    SRV_ID floorNormSRV = resourceMan->LoadSRV( context, L"Assets/Textures/floor_normals.png" );
    SRV_ID floorRoughnessMap = resourceMan->LoadSRV( context, L"Assets/Textures/floor_roughness.png" );
    SRV_ID floorMetalMap = resourceMan->LoadSRV( context, L"Assets/Textures/floor_metal.png" );
    Material_ID floorMatID = resourceMan->LoadMaterial( vertexShader, pixelShader, floorDif, floorNormSRV, floorRoughnessMap, floorMetalMap, SamplerID );

    XMFLOAT3 floorPos = XMFLOAT3( 0.f, -5.f, 0.f );
    Entity_ID floorID = entityMan->AddEntity(
        resourceMan->GetMesh( CubeMeshID ), resourceMan->GetMaterial( floorMatID ), floorPos );

    entityMan->GetEntity( floorID )->SetScale( XMFLOAT3( 5.f, 5.f, 5.f ) );
    entityMan->GetEntity( floorID )->SetPhysicsLayer( EPhysicsLayer::STATIC );


    // Load in the skybox SRV --------------------------------------------------------
    SkyboxSrvID = resourceMan->LoadSRV_DDS( context, L"Assets/Textures/SunnyCubeMap.dds" );
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
    // Handle base-level DX resize stuff
    DXCore::OnResize();

    FlyingCamera->UpdateProjectionMatrix( width, height );
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update( float deltaTime, float totalTime )
{
    // Quit if the escape key is pressed
    if ( GetAsyncKeyState( VK_ESCAPE ) )
        Quit();

    // Update the camera
    FlyingCamera->Update( deltaTime );

    static float currentSpeed = 1.f;
    static float amountMoved = 0.f;
    const float target = 10.0f;

    for ( size_t i = 0; i < PointLights.size(); ++i )
    {
        XMFLOAT3 newPos = PointLights[ i ].Position;
        XMFLOAT3 acceleration = {};


        if ( i % 2 == 0 )
        {
            newPos.x += LightMoveSpeed * currentSpeed * deltaTime;
        }
        else
        {
            newPos.x -= LightMoveSpeed * currentSpeed * deltaTime;
        }

        amountMoved += LightMoveSpeed * currentSpeed * deltaTime;

        if ( amountMoved > 5.f || amountMoved < -5.f )
        {
            amountMoved = 0.f;
            currentSpeed *= -1.f;
        }

        PointLights[ i ].Position = newPos;
    }


}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw( float deltaTime, float totalTime )
{
    // Background color (Cornflower Blue in this case) for clearing

    //const float color[ 4 ] = { 0.4f, 0.6f, 0.75f, 0.0f };
    //const float color[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of Draw (before drawing *anything*)
    context->ClearRenderTargetView( backBufferRTV, ( float* ) ( &BackgroundColor ) );
    context->ClearDepthStencilView(
        depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0 );

    // Set buffers in the input assembler
    //  - Do this ONCE PER OBJECT you're drawing, since each object might
    //    have different geometry.
    UINT stride = sizeof( Vertex );
    UINT offset = 0;

    Mesh* EnMesh = nullptr;
    ID3D11Buffer* VertBuff = nullptr;

    Entity* CurrentEntity = entityMan->GetEntity( 0 );

    for ( size_t i = 0; i < entityMan->GetEntityCount(); ++i )
    {
        CurrentEntity = entityMan->GetEntity( i );

        if ( !CurrentEntity->GetIsActive() ) continue;

        // Send lighting info ---------------------------------------------------------
        if ( DirLights.size() > 0 )
        {
            pixelShader->SetData( "DirLights", ( void* ) ( &DirLights[ 0 ] ), sizeof( DirectionalLight ) * MAX_DIR_LIGHTS );
        }
        pixelShader->SetInt( "DirLightCount", ( UseDirLights ? static_cast< int >( DirLights.size() ) : 0 ) );

        if ( PointLights.size() > 0 )
        {
            pixelShader->SetData( "PointLights", ( void* ) ( &PointLights[ 0 ] ), sizeof( PointLight ) * MAX_POINT_LIGHTS );
        }
        pixelShader->SetInt( "PointLightCount", ( UsePointLights ? static_cast< int >( PointLights.size() ) : 0 ) );


        // Send camera info ---------------------------------------------------------
        pixelShader->SetFloat3( "CameraPosition", FlyingCamera->GetPosition() );
        CurrentEntity->PrepareMaterial( FlyingCamera->GetViewMatrix(), FlyingCamera->GetProjectMatrix() );

        // Draw the entity ---------------------------------------------------------
        EnMesh = CurrentEntity->GetEntityMesh();
        VertBuff = EnMesh->GetVertexBuffer();

        context->IASetVertexBuffers( 0, 1, &VertBuff, &stride, &offset );
        context->IASetIndexBuffer( EnMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 );

        // Finally do the actual drawing
        context->DrawIndexed( CurrentEntity->GetEntityMesh()->GetIndexCount(), 0, 0 );
    }

    // Draw the Sky box -------------------------------------

    if ( DrawSkyBox )
    {
        // Set up sky render states
        context->RSSetState( skyRastState );
        context->OMSetDepthStencilState( skyDepthState, 0 );

        // After drawing all of our regular (solid) objects, draw the sky!
        ID3D11Buffer* skyVB = CurrentEntity->GetEntityMesh()->GetVertexBuffer();
        ID3D11Buffer* skyIB = CurrentEntity->GetEntityMesh()->GetIndexBuffer();

        // Set the buffers
        context->IASetVertexBuffers( 0, 1, &skyVB, &stride, &offset );
        context->IASetIndexBuffer( skyIB, DXGI_FORMAT_R32_UINT, 0 );

        SkyBoxVS->SetMatrix4x4( "view", FlyingCamera->GetViewMatrix() );
        SkyBoxVS->SetMatrix4x4( "projection", FlyingCamera->GetProjectMatrix() );

        SkyBoxVS->CopyAllBufferData();
        SkyBoxVS->SetShader();

        // Send texture-related stuff
        SkyBoxPS->SetShaderResourceView( "SkyTexture", resourceMan->GetSRV( SkyboxSrvID ) );
        SkyBoxPS->SetSamplerState( "BasicSampler", resourceMan->GetSampler( SamplerID ) );

        SkyBoxPS->CopyAllBufferData(); // Remember to copy to the GPU!!!!
        SkyBoxPS->SetShader();

        // Reset any changed render states!
        context->RSSetState( skyRastState );
        context->OMSetDepthStencilState( skyDepthState, 0 );

        // Draw the skybox "mesh"
        context->DrawIndexed( resourceMan->GetMesh( CubeMeshID )->GetIndexCount(), 0, 0 );

        // Reset any states we've changed for the next frame!
        context->RSSetState( 0 );
        context->OMSetDepthStencilState( 0, 0 );
    }


#if defined( _DEBUG ) ||  defined( DRAW_LIGHTS )

    if ( DrawLightGizmos )
        DrawLightSources();

#endif // _DEBUG

#if defined(ENABLE_UI)

    DrawUI();

#endif  // ENABLE_UI

    // Present the back buffer to the user
    //  - Puts the final frame we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
    swapChain->Present( 0, 0 );
    //swapChain->Present( 1, 0 );   // Present with vsync
}


void Game::DrawLightSources()
{
    Mesh* lightMesh = ResourceManager::GetInstance()->GetMesh( PointLightMesh_ID );
    ID3D11Buffer * vb = lightMesh->GetVertexBuffer();
    ID3D11Buffer * ib = lightMesh->GetIndexBuffer();
    unsigned int indexCount = lightMesh->GetIndexCount();

    // Turn on these shaders
    vertexShader->SetShader();
    UnlitPixelShader->SetShader();

    // Set up vertex shader
    vertexShader->SetMatrix4x4( "view", FlyingCamera->GetViewMatrix() );
    vertexShader->SetMatrix4x4( "projection", FlyingCamera->GetProjectMatrix() );

    for ( size_t i = 0; i < PointLights.size(); ++i )
    {
        PointLight light = PointLights[ i ];
        // Set buffers in the input assembler
        UINT stride = sizeof( Vertex );
        UINT offset = 0;
        context->IASetVertexBuffers( 0, 1, &vb, &stride, &offset );
        context->IASetIndexBuffer( ib, DXGI_FORMAT_R32_UINT, 0 );

        float scale = light.Range / 13.0f;

        XMMATRIX rotMat = XMMatrixIdentity();
        XMMATRIX scaleMat = XMMatrixScaling( scale, scale, scale );
        XMMATRIX transMat = XMMatrixTranslation( light.Position.x, light.Position.y, light.Position.z );

        // Make the transform for this light
        XMFLOAT4X4 world;
        XMStoreFloat4x4( &world, XMMatrixTranspose( scaleMat * rotMat * transMat ) );

        // Set up the world matrix for this light
        vertexShader->SetMatrix4x4( "world", world );

        // Set up the pixel shader data
        XMFLOAT3 finalColor = light.Color;
        finalColor.x *= light.Intensity;
        finalColor.y *= light.Intensity;
        finalColor.z *= light.Intensity;
        UnlitPixelShader->SetFloat3( "Color", finalColor );

        // Copy data
        vertexShader->CopyAllBufferData();
        UnlitPixelShader->CopyAllBufferData();

        // Draw
        context->DrawIndexed( indexCount, 0, 0 );

    }

}

void Game::DrawUI()
{
#if defined( ENABLE_UI )
    // Create a new IMGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Draw the UI options here -----------------------------------

    {   // Options --------------------------
        ImGui::Begin( "Options" );

        ImGui::Checkbox( "Use Dir Lights", &UseDirLights );

        ImGui::Checkbox( "Use Point Lights", &UsePointLights );
        ImGui::Checkbox( "Draw Light Gizmos", &DrawLightGizmos );

        ImGui::Checkbox( "Use SkyBox", &DrawSkyBox );

        if ( !DrawSkyBox )
            ImGui::ColorEdit3( "Background Color", ( float* ) ( &BackgroundColor ) ); // Edit 3 floats representing a color

        // Lighting settings
        ImGui::SliderFloat( "Light Move Speed", &LightMoveSpeed, 0.0f, 1.0f );

        ImGui::End();   // If you want more than one window, then use ImGui::Beigin
    }

    {   // Stats and Info ---------------------------
        ImGui::Begin( "Info" );
        ImGui::Text( "%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate );
        ImGui::Text( "%.1f FPS", ImGui::GetIO().Framerate );

        ImGui::Text( "R.Click - Rotate" );
        ImGui::Text( "WASD    - Move" );
        ImGui::Text( "Space   - Go Up" );
        ImGui::Text( "X       - Go Down" );

        ImGui::End();
    }

    {   // Draw the hierarchy of objects --------------------------
        ImGui::Begin( "Hierarchy" );

        Entity* CurrentEntity = nullptr;

        for ( size_t i = 0; i < entityMan->GetEntityCount(); ++i )
        {
            CurrentEntity = entityMan->GetEntity( i );

            ImGui::Text( CurrentEntity->GetName().c_str() );
        }

        ImGui::End();
    }

    {   // Inspector --------------------------
        if ( SelectedEntity != nullptr )
        {
            ImGui::Begin( "Inspector" );

            ImGui::Text( SelectedEntity->GetName().c_str() );
            XMFLOAT3 newPos = SelectedEntity->GetPosition();

            ImGui::InputFloat3( "Position ", ( float* ) &newPos );

            // The position of the current object
            SelectedEntity->SetPosition( newPos );

            ImGui::End();
        }
    }


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
#endif
}



#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnLookDown()
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
void Game::OnLookUp()
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
#pragma endregion