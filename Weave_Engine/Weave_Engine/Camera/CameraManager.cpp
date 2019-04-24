#include "stdafx.h"

#include "CameraManager.h"
#include "Entity/Entity.h"
#include "Camera.h"
#include "../Scripting/ScriptComponent.h"

CameraManager* CameraManager::Instance = nullptr;

CameraManager* CameraManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new CameraManager();
    }
    return Instance;
}

void CameraManager::Init()
{
    assert( Instance != nullptr );
    // Create a camera by default
    Instance->CreateDebugCamera();
}

void CameraManager::ReleaseInstance()
{
    SAFE_DELETE( Instance );
}

CameraManager::CameraManager()
{
    LOG_TRACE( "Camera man created" );
}

CameraManager::~CameraManager()
{
    LOG_TRACE( "Camera man dtor" );
    
    SAFE_DELETE( DebugCameraEntity );
    DebugCamera = nullptr;
    CurrentCameras.clear();
}

void CameraManager::CreateDebugCamera()
{
    // Create an entity 
    // This should not happen here
    DebugCameraEntity = new Entity( "Default_Editor_Cam" );
    DebugCameraEntity->SetIsDestroyableOnLoad( false );
   
    // Add a camera component
    DebugCamera = DebugCameraEntity->AddComponent<Camera>();
    if ( ActiveCamera == nullptr )
    {
        ActiveCamera = DebugCamera;
    }

    std::string flyingCamScript = "Assets/Scripts/FlyingCamera.lua";
    FileName name( flyingCamScript.begin(), flyingCamScript.end() );
    DebugCameraEntity->AddComponent<ScriptComponent>( name );
}

void CameraManager::RegisterCamera( const size_t aID, Camera* aCam )
{
    // Ensure that this camera does not exist in the map
    auto it = CurrentCameras.find( aID );
    if ( it == CurrentCameras.end() )
    {
        CurrentCameras [ aID ] = aCam;
        LOG_TRACE( "Registered camera! {}", aID );
    }
}

void CameraManager::UnregisterCamera( const size_t aID )
{
    if ( aID == DebugCamera->GetComponentId() ) return;

    auto it = CurrentCameras.find( aID );
    if ( it != CurrentCameras.end() )
    {
        // If this is the camera currently in use, default to the editor cam
        if ( aID == ActiveCamera->GetCameraID() )
        {
            ActiveCamera = DebugCamera;
        }
        CurrentCameras.erase( aID );
    }
}

void CameraManager::SetActiveCamera( const size_t aCamID )
{
    // Set this camera as the active one if it is in the map
    auto it = CurrentCameras.find( aCamID );
    if ( it != CurrentCameras.end() )
    {
        ActiveCamera = CurrentCameras [ aCamID ];
    }
}