#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"

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

#if defined(DEBUG) || defined(_DEBUG)
    // Do we want a console window?  Probably only in debug mode
    CreateConsoleWindow( 500, 120, 32, 120 );
    printf( "Console window created successfully.  Feel free to printf() here." );
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
    // Delete our simple shader objects, which
    // will clean up their own internal DirectX stuff
    delete vertexShader;
    delete pixelShader;

    EntityManager::ReleaseInstance();

    ResourceManager::ReleaseInstance();

    delete FlyingCamera;

    InputManager::Release();

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
    EntityManager::GetInstance();

    ResourceManager::Initalize( device );

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
}

void Game::InitLights()
{
    DirectLight.AmbientColor = XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f );  // Ambient color is the color when we are in shadow
    DirectLight.DiffuseColor = XMFLOAT4( 0.9f, 0.1f, 0.1f, 1.0f );
    DirectLight.Direction = XMFLOAT3( 1.0f, 0.0f, 0.0f );

    DirectLight_Two.AmbientColor = XMFLOAT4( 0.1f, 0.1f, 0.1f, 1.0f );
    DirectLight_Two.DiffuseColor = XMFLOAT4( 0.0f, 1.0f, 0.1f, 1.0f );
    DirectLight_Two.Direction = XMFLOAT3( -1.0f, 0.0f, 0.5f );
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
    ResourceManager* resources = ResourceManager::GetInstance();
    UINT meshID = resources->LoadMesh( "Assets/Models/sphere.obj" );

    UINT diffSRV = resources->LoadSRV( context, L"Assets/Textures/BeachPebbles_1024_albedo.tif" );
    UINT normSRV = resources->LoadSRV( context, L"Assets/Textures/BeachPebbles_1024_normal.tif" );

    D3D11_SAMPLER_DESC samplerDesc = {}; // Zero out the struct memory
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    UINT samplerID = resources->AddSampler( samplerDesc );

    UINT matID = resources->LoadMaterial( vertexShader, pixelShader, diffSRV, normSRV, samplerID );

    EntityManager::GetInstance()->AddEntity( 
        resources->GetMesh( meshID ), resources->GetMaterial( matID ) );

    resources = nullptr;

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

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw( float deltaTime, float totalTime )
{
    // Background color (Cornflower Blue in this case) for clearing
    //const float color[ 4 ] = { 0.4f, 0.6f, 0.75f, 0.0f };
    const float color[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of Draw (before drawing *anything*)
    context->ClearRenderTargetView( backBufferRTV, color );
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
    Entity* CurrentEntity = EntityManager::GetInstance()->GetEntity( 0 );



    // Draw the entities

    // Calculate the world matrix ------------------------------------------
    pixelShader->SetData(
        "light_two",
        &DirectLight_Two,
        sizeof( DirectionalLight )
    );

    // Set the pixel shader before we draw each entity
    pixelShader->SetData(
        "light",
        &DirectLight,
        sizeof( DirectionalLight )
    );


    pixelShader->SetFloat3( "CameraPosition", FlyingCamera->GetPosition() );

    CurrentEntity->PrepareMaterial( FlyingCamera->GetViewMatrix(), FlyingCamera->GetProjectMatrix() );

    // Draw the entity ---------------------------------------------------------
    EnMesh = CurrentEntity->GetEntityMesh();
    VertBuff = EnMesh->GetVertexBuffer();

    context->IASetVertexBuffers( 0, 1, &VertBuff, &stride, &offset );
    context->IASetIndexBuffer( EnMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 );

    context->DrawIndexed(
        EnMesh->GetIndexCount(),
        0,
        0 );


    // Present the back buffer to the user
    //  - Puts the final frame we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
    swapChain->Present( 0, 0 );
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown( WPARAM buttonState, int x, int y )
{
    // Add any custom code here...
    FlyingCamera->SetDoRotation( true );

    // Save the previous mouse position, so we have it for the future
    prevMousePos.x = x;
    prevMousePos.y = y;

    // Caputure the mouse so we keep getting mouse move
    // events even if the mouse leaves the window.  we'll be
    // releasing the capture once a mouse button is released
    SetCapture( hWnd );
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp( WPARAM buttonState, int x, int y )
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