#pragma once

//#define ENABLE_UI

#if defined(_DEBUG)  && defined(ENABLE_UI)

#include "../Utils/imgui/imgui.h"
#include "../Utils/imgui/imgui_impl_win32.h"
#include "../Utils/imgui/imgui_impl_dx11.h"

#endif

#include "DebugSettings.h"

#include "DXCore.h"
#include "../Resources/SimpleShader.h"
#include <DirectXMath.h>
#include <vector>
#include "InputManager.h"
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
    void OnMouseDown( WPARAM buttonState, int x, int y );
    void OnMouseUp( WPARAM buttonState, int x, int y );
    void OnMouseMove( WPARAM buttonState, int x, int y );
    void OnMouseWheel( float wheelDelta, int x, int y );

private:

    // Initialization helper methods - feel free to customize, combine, etc.
    void LoadShaders();
    void CreateMatrices();
    void CreateBasicGeometry();
    void InitLights();
    void CreatUI();

    void DrawLightSources();
    void DrawUI();

    
    // Wrappers for DirectX shaders to provide simplified functionality
    SimpleVertexShader* vertexShader = nullptr;
    SimplePixelShader* pixelShader = nullptr;
    SimplePixelShader* UnlitPixelShader = nullptr;

    UINT SamplerID;
    UINT SkyboxSrvID;

    // Skybox resources
    SimpleVertexShader* SkyBoxVS = nullptr;
    SimplePixelShader* SkyBoxPS = nullptr;
    ID3D11RasterizerState* skyRastState = nullptr;
    ID3D11DepthStencilState* skyDepthState = nullptr;

    bool UseDirLights = true;
    bool DrawPhysicsCollider = true;

    const float Gravity = -0.001f;

    // Flying camera for initial testing
    Camera* FlyingCamera = nullptr;

    // Lights
    std::vector<DirectionalLight> DirLights;
    std::vector<PointLight> PointLights;
    UINT PointLightMesh_ID = 0;

    // Keeps track of the old mouse position.  Useful for 
    // determining how far the mouse moved in a single frame.
    POINT prevMousePos;
};

