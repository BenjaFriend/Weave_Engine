#pragma once



#include "../stdafx.h"

#include <Windows.h>
#include <d3d11.h>
#include <string>

#include "../Input/InputManager.h"
#include "../Input/Input_Win.h"

#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"
//#include "../Entity/EntityManager.h"
#include "../Resources/ResourceManager.h"
#include "../ECS/ComponentManager.h"
#include "../Physics/PhysicsManager.h"

// We can include the correct library files here
// instead of in Visual Studio settings if we want
#pragma comment(lib, "d3d11.lib")

#include "../Scripting/ScriptUtils.h"

#if defined( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

// The editor depends on IMGui being enabled
#if defined( EDITOR_ON )

#include "../Editor/EditorCore.h"

#endif  // EDITOR_ON

#endif  // ENABLE_UI

class DXCore
{
public:
    DXCore(
        HINSTANCE hInstance,		// The application's handle
        char* titleBarText,			// Text for the window's title bar
        unsigned int windowWidth,	// Width of the window's client area
        unsigned int windowHeight,	// Height of the window's client area
        bool debugTitleBarStats );	// Show extra stats (fps) in title bar?
    ~DXCore();

    // Static requirements for OS-level message processing
    static DXCore* DXCoreInstance;
    static LRESULT CALLBACK WindowProc(
        HWND hWnd,		// Window handle
        UINT uMsg,		// Message
        WPARAM wParam,	// Message's first parameter
        LPARAM lParam	// Message's second parameter
    );

    // Internal method for message handling
    LRESULT ProcessMessage( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // Initialization and game-loop related methods
    HRESULT InitWindow();
    HRESULT InitDirectX();
    HRESULT Run();
    virtual void Quit();
    virtual void OnResize();

    // Pure virtual methods for setup and game functionality
    virtual void Init() = 0;
    virtual void Update( float deltaTime, float totalTime ) = 0;
    virtual void Draw( float deltaTime, float totalTime ) = 0;

    // Convenience methods for handling mouse input, since we
    // can easily grab mouse input from OS-level messages
    virtual void OnMouseMove( WPARAM buttonState, int x, int y ) {}
    virtual void OnMouseWheel( float wheelDelta, int x, int y ) {}

protected:
    HINSTANCE	hInstance;		// The handle to the application
    HWND		hWnd;			// The handle to the window itself
    std::string titleBarText;	// Custom text in window's title bar
    bool		titleBarStats;	// Show extra stats in title bar?

    // Size of the window's client area
    unsigned int width;
    unsigned int height;

    // DirectX related objects and variables
    D3D_FEATURE_LEVEL		dxFeatureLevel;
    IDXGISwapChain*			swapChain = nullptr;
    ID3D11Device*			device = nullptr;
    ID3D11DeviceContext*	context = nullptr;

    ID3D11RenderTargetView* backBufferRTV = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;

    Input::InputManager* inputManager = nullptr;
    SceneManagement::SceneManager* sceneManager = nullptr;
    Logger* logger = nullptr;
    ResourceManager* resourceMan = nullptr;
    EntityManager* entityMan = nullptr;
    ECS::ComponentManager* ComponentMan = nullptr;
    Physics::PhysicsManager* PhysicsMan = nullptr;
    Scripting::ScriptManager* ScriptMan = nullptr;

#if defined(EDITOR_ON)

    Editor::EditorCore* editor = nullptr;

#endif

    // Helper function for allocating a console window
    void CreateConsoleWindow( int bufferLines, int bufferColumns, int windowLines, int windowColumns );

private:
    // Timing related data
    double perfCounterSeconds;
    float totalTime;
    float deltaTime;
    __int64 startTime;
    __int64 currentTime;
    __int64 previousTime;

    // FPS calculation
    int fpsFrameCount;
    float fpsTimeElapsed;

    void InitSystems();

    void UpdateTimer();			// Updates the timer for this frame(
    
    void UpdateTitleBarStats();	// Puts debug info in the title bar
     
};  // DXCore class