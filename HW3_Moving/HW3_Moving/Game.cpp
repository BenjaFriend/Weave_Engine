#include "Game.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Entity.h"

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
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

	vertexShader = 0;
	pixelShader	 = 0;
	EntityCount	 = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
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

	// Delete the entities
	for (size_t i = 0; i < EntityCount; ++i)
	{
		Entity* CurEntity = Entities[i];
		if (CurEntity) delete CurEntity;
	}

	Entities.clear();
	EntityCount = 0;
	
	// Delete the mesh
	delete TestMesh1;
	delete TestMesh2;
	delete TestMesh3;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices1[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), green },
	};

	Vertex vertices2[] =
	{
		{ XMFLOAT3(+1.0f, +2.0f, +1.0f), red },
		{ XMFLOAT3(+2.5f, +0.0f, +1.0f), red },
		{ XMFLOAT3(-0.5f, +0.0f, +1.0f), red },
	};

	Vertex vertices3[] =
	{
		{ XMFLOAT3(-1.0f, +0.0f, -1.0f), green},
		{ XMFLOAT3(+0.5f, -2.0f, -1.0f), green },
		{ XMFLOAT3(-2.5f, -2.0f, -1.0f), green },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT indices[] = { 0, 1, 2 };


	TestMesh1 = new Mesh(device, vertices1, 3, indices, 3);
	TestMesh2 = new Mesh(device, vertices2, 3, indices, 3);
	TestMesh3 = new Mesh(device, vertices3, 3, indices, 3);

	// Create an entity based on these meshes
	Entities.push_back( new Entity(TestMesh2));
	++EntityCount;

	Entities.push_back( new Entity(TestMesh2));
	++EntityCount;

	Entities.push_back(new Entity(TestMesh1));
	++EntityCount;
	Entities[EntityCount - 1]->MoveAbsolute(-1.f, -1.f, 0.f);


	Entities.push_back(new Entity(TestMesh3));
	++EntityCount;
	Entities[EntityCount - 1]->SetPosition(3.f, 0.f, 6.f);

	Entities.push_back(new Entity(TestMesh3));
	++EntityCount;
	Entities[EntityCount - 1]->SetScale(0.5f, 0.5f, 0.5f);

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	XMFLOAT3 DeltaInput = XMFLOAT3(0.f, 0.f, 0.f);

	if (GetAsyncKeyState('W') & 0x80000)
	{
		// Move forward
		DeltaInput.y += 1.f;
	}
	if (GetAsyncKeyState('S') & 0x80000)
	{
		// Move down
		DeltaInput.y -= 1.f;
	}
	if (GetAsyncKeyState('A') & 0x80000)
	{
		// Move left
		DeltaInput.x -= 1.f;
	}
	if (GetAsyncKeyState('D') & 0x80000)
	{
		// Move right
		DeltaInput.x += 1.f;
	}

	// Update the entity scale and position
	if (EntityCount > 0)
	{
		const float Speed = 1.2f;
		float SinTime = (0.5f * sinf(0.5f * totalTime) + 0.8f);

		Entities[0]->SetScale(SinTime, SinTime , SinTime );

		Entities[0]->MoveAbsolute(
			DeltaInput.x * Speed * deltaTime,
			DeltaInput.y * Speed * deltaTime,
			DeltaInput.z * Speed * deltaTime 
		);
	}

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	Mesh* EnMesh			 = nullptr;
	ID3D11Buffer* VertBuff	 = nullptr;
	Entity* CurrentEntity	 = nullptr;

	// Draw the entities
	for (size_t i = 0; i < EntityCount; ++i)
	{
		// Calculate the world matrix ------------------------------------------
		CurrentEntity = Entities[i];

		EnMesh = CurrentEntity->GetEntityMesh();
		VertBuff = EnMesh->GetVertexBuffer();

		const XMFLOAT3 f3Scale = CurrentEntity->GetScale();
		XMMATRIX Scale = XMMatrixScaling(f3Scale.x, f3Scale.y, f3Scale.z);

		// Make the sin
		XMMATRIX Rot = XMMatrixRotationX(CurrentEntity->GetRotation().x);

		const XMFLOAT3 f3Pos = CurrentEntity->GetPosition();
		XMMATRIX Pos = XMMatrixTranslation(f3Pos.x, f3Pos.y, f3Pos.z);

		// Calculate the world matrix
		XMMATRIX WorldMM = Scale * Rot * Pos;

		// Store this info in the actual 4x4 matrix
		XMFLOAT4X4 World4x4;
		XMStoreFloat4x4(&World4x4, XMMatrixTranspose(WorldMM));	// Don't forget to transpose!

		vertexShader->SetMatrix4x4("world", World4x4);
		vertexShader->SetMatrix4x4("view", viewMatrix);
		vertexShader->SetMatrix4x4("projection", projectionMatrix);

		// Draw the entity ---------------------------------------------------------
		VertBuff = EnMesh->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &VertBuff, &stride, &offset);
		context->IASetIndexBuffer(EnMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			EnMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
				   // Tell the shader to send data to the GPU in one big chunk
		vertexShader->CopyAllBufferData();

	}
	



	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion