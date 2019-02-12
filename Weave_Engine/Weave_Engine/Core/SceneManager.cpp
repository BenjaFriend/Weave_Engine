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
    // We need to make sure that we unload the current scene first
    UnloadCurrentScene();

    // Parse the scene file and load in the given settings / entities
    std::ifstream ifs( aSceneName.c_str() );

    if ( ifs.is_open() )
    {
        // Store the info in the scene file in the JSON object
        nlohmann::json njson;
        ifs >> njson;

        ActiveScene = njson [ SCENE_NAME_SAVE_KEY ];
        LOG_TRACE( "Active Scene: {}", ActiveScene );

        nlohmann::json entityArray = njson [ ENTITY_ARRAY_SAVE_KEY ];

        nlohmann::json::iterator it = entityArray.begin();

        for ( ; it != entityArray.end(); ++it )
        {
            // Key is the name 
            std::string ent_name = ( *it ) [ NAME_SAVE_KEY ];
            bool ent_active = ( *it ) [ IS_ACTIVE_SAVE_KEY ];

            LOG_TRACE( "Entity name: {} Is Active: {}", ent_name, ent_active );
            // Load it in with the entity manager! 
            // This isn't gonna work with components right now
        }
    }
    else
    {
        //LOG_ERROR( "Failed to load scene: {}", SceneFile );
    }

    ifs.close();



    // Let any listeners know that the scene has been loaded
    OnSceneLoadDispatcher.Dispatch();
}

void SceneManager::UnloadCurrentScene()
{
    // Unload all entities
    OnSceneUnloadDispatcher.Dispatch();
}