#include "../stdafx.h"
#include "SceneManager.h"

using namespace SceneManagement;
SceneManager* SceneManager::Instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new SceneManager();
    }
    return Instance;
}

void SceneManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
    }
}

SceneManager::SceneManager()
{
    LOG_TRACE( "Created scene manager!" );
}

SceneManager::~SceneManager()
{
    UnloadCurrentScene();

    // Remove any listeners to the scene management
}

void SceneManager::LoadScene( FileName & aSceneName )
{
    // Parse the scene file and load in the given settings / entities
}

void SceneManager::UnloadCurrentScene()
{
    // Unload all entities
}