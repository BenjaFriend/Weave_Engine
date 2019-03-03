#include "../stdafx.h"

#include "CameraManager.h"

// #TODO: Implement the camera manager
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
    AddCamera( "Default_Editor_Cam" );
}

CameraManager::~CameraManager()
{
    LOG_TRACE( "Camera man dtor" );
    
    // Delete all cameras
    //if ( CameraEntity != nullptr )
    //{
    //    delete CameraEntity;
    //    CameraEntity = nullptr;
    //    ActiveCamera = nullptr;
    //}
}

Camera* CameraManager::AddCamera( const std::string aName )
{
    // Create an entity 
    //Entity* CameraEnt = new Entity( aName );
    //CameraEnt->SetIsDestroyableOnLoad( false );
    //
    //// Add a camera component
    //Camera* CamComp = CameraEnt->AddComponent<Camera>();
    //
    ////if ( CameraEntity == nullptr )
    ////{
    ////    CameraEntity = CameraEnt;
    ////    ActiveCamera = CamComp;
    ////}
    //
    ////CurrentCameras.emplace( aName, CamComp );

    return nullptr;
}
