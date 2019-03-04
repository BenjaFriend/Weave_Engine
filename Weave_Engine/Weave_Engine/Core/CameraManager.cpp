#include "../stdafx.h"

#include "CameraManager.h"
#include "../Entity/Entity.h"
#include "../Entity/Camera.h"

CameraManager* CameraManager::Instance = nullptr;

CameraManager* CameraManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new CameraManager();
    }
    return Instance;
}

void CameraManager::ReleaseInstance()
{
    SAFE_DELETE( Instance );
}

CameraManager::CameraManager()
{
    LOG_TRACE( "Camera man created" );
    // Create a camera by default
    CreateDebugCamera();
    
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
}

Camera* CameraManager::AddCamera( const std::string aName )
{
    // Create an entity 
    // This should not happen here
    Entity* CameraEnt = new Entity( aName );
    CameraEnt->SetIsDestroyableOnLoad( false );
    
    // Add a camera component
    Camera* CamComp = CameraEnt->AddComponent<Camera>();
    
    return CamComp;
}