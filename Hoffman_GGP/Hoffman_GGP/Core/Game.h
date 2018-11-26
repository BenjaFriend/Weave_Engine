#pragma once

#include "../stdafx.h"

#include "DXCore.h"
#include "../Resources/SimpleShader.h"
#include <DirectXMath.h>
#include <vector>
#include <thread>
#include <chrono>
#include "../Entity/EntityManager.h"
#include "../Resources/ResourceManager.h"
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
    void OnLookDown();
    void OnLookUp();

    void OnMouseMove( WPARAM buttonState, int x, int y );
    void OnMouseWheel( float wheelDelta, int x, int y );

private:

    // Initialization helper methods - feel free to customize, combine, etc.
    void LoadShaders();
    void CreateMatrices();
    void CreateBasicGeometry();
    void InitLights();

    void DrawLightSources();
    void DrawUI();

    
    // Wrappers for DirectX shaders to provide simplified functionality
    SimpleVertexShader* vertexShader = nullptr;
    SimplePixelShader* pixelShader = nullptr;
    SimplePixelShader* UnlitPixelShader = nullptr;

    Sampler_ID SamplerID;
    SRV_ID SkyboxSrvID;
    Mesh_ID CubeMeshID;

    // Skybox resources
    SimpleVertexShader* SkyBoxVS = nullptr;
    SimplePixelShader* SkyBoxPS = nullptr;
    ID3D11RasterizerState* skyRastState = nullptr;
    ID3D11DepthStencilState* skyDepthState = nullptr;

    bool UseDirLights = true;
    bool DrawLightGizmos = true;
    bool DrawPhysicsCollider = true;
    bool MovePointLights = true;
    bool DrawSkyBox = true;

    const float Gravity = -0.001f;

    ImVec4 BackgroundColor;

    // Flying camera for initial testing
    Camera* FlyingCamera = nullptr;

    // managers
    ResourceManager* resourceMan = nullptr;
    EntityManager* entityMan = nullptr;

    // Lights
    std::vector<DirectionalLight> DirLights;
    std::vector<PointLight> PointLights;
    Mesh_ID PointLightMesh_ID = 0;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT prevMousePos;
};

