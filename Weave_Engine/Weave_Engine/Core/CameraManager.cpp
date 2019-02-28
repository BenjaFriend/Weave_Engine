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
    if ( Instance != nullptr )
    {
        delete Instance;
    }
}

CameraManager::CameraManager()
{
    LOG_TRACE( "Camera man created" );
}

CameraManager::~CameraManager()
{
    LOG_TRACE( "Camera man dtor" );

}

Camera* CameraManager::AddCamera()
{
    return nullptr;
}
