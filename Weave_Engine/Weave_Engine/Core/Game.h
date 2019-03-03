#pragma once

#include "../stdafx.h"

#include "DXCore.h"
#include "../Resources/SimpleShader.h"
#include <vector>
#include <thread>
#include <chrono>

#include "../Lighting/LightShaderDefs.h"
#include "../Physics/Collisions.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Entity;
class Camera;
class Material;

class Game
    : public DXCore
{

public:
    Game( HINSTANCE hInstance );
    ~Game();

    // Overridden setup and game loop methods, which
    // will be called automatically
    void Init();
    void OnResize();
    void Update( float deltaTime, float totalTime );
    void Draw( float deltaTime, float totalTime );

    // Overridden mouse input helper methods
    void OnMouseDown();
    void OnMouseUp();

    void OnMouseMove( WPARAM buttonState, int x, int y );
    void OnMouseWheel( float wheelDelta, int x, int y );
    virtual void Quit() override;

private:

    // Initialization helper methods - feel free to customize, combine, etc.
    void LoadShaders();
    void CreateMatrices();
    void CreateBasicGeometry();
    void InitLights();

    void DrawLightSources();
    void DrawColliders();

    /// <summary>
    /// Draw any game options
    /// </summary>
    void DrawUI();


    // Wrappers for DirectX shaders to provide simplified functionality
    SimpleVertexShader* vertexShader = nullptr;
    SimplePixelShader* pixelShader = nullptr;
    SimplePixelShader* UnlitPixelShader = nullptr;

    Sampler_ID SamplerID;
    ID3D11RasterizerState* WireFrame = nullptr;

    ID3D11ShaderResourceView* SkyboxSrvID = nullptr;
    Mesh* CubeMesh = nullptr;

    // Skybox resources
    SimpleVertexShader* SkyBoxVS = nullptr;
    SimplePixelShader* SkyBoxPS = nullptr;
    ID3D11RasterizerState* skyRastState = nullptr;
    ID3D11DepthStencilState* skyDepthState = nullptr;

    float BackgroundColor [ 4 ] = { 0.45f, 0.55f, 0.60f, 1.00f };

    // Flying camera for initial testing
	Camera* FlyingCamera = nullptr;
    Mesh* PointLightMesh = nullptr;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT prevMousePos;

    // Demo Options
    bool UseDirLights = true;
    bool UsePointLights = true;

    bool DrawLightGizmos = true;
    bool DrawSkyBox = true;
    bool DebugDrawColliders = true;
};