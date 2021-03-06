#include "stdafx.h"
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
    ActiveScene = new Scene();
}

SceneManager::~SceneManager()
{
    UnloadCurrentScene();
    if ( ActiveScene != nullptr )
    {
        delete ActiveScene;
        ActiveScene = nullptr;
    }
    // Remove any listeners to the scene management
}

void SceneManager::LoadScene( const FileName & aSceneName )
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

        std::string sceneName = njson [ SCENE_NAME_SAVE_KEY ];
        LOG_TRACE( "SceneName: {}", sceneName );
     
        ActiveScene->SetSceneName( sceneName );

        nlohmann::json entityArray = njson [ ENTITY_ARRAY_SAVE_KEY ];

        nlohmann::json::iterator it = entityArray.begin();

        for ( ; it != entityArray.end(); ++it )
        {
            // Load it in with the entity manager! 
            ActiveScene->AddEntityFromfile( *it );
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
    LOG_TRACE( "Unload Current Scene" );
    // Unload all entities
    OnSceneUnloadDispatcher.Dispatch();
    ActiveScene->ResetScene();
    LOG_TRACE( "Done unloading current scene!" );
}

void SceneManager::SaveScene( FileName & aSceneFile )
{
    LOG_TRACE( "Save scene!" );
    nlohmann::json njson;

    njson [ SCENE_NAME_SAVE_KEY ] = "Test_Scene_Name";
    njson [ ENTITY_ARRAY_SAVE_KEY ] = nlohmann::json::array();

    Entity* CurrentEntity = nullptr;

    SceneManagement::Scene* CurScene = GetActiveScene();
    Entity* entArray = CurScene->GetEntityArray();

    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        CurrentEntity = &entArray [ i ];
        if ( CurrentEntity != nullptr && CurrentEntity->GetIsInUse() )
        {
            CurrentEntity->SaveObject( njson [ ENTITY_ARRAY_SAVE_KEY ] );
        }
    }

    // Open a file stream to the given scene file
    std::ofstream ofs( aSceneFile );
    if ( ofs.is_open() )
    {
        ofs << std::setw( 4 ) << njson << std::endl;
    }
    else
    {
        LOG_ERROR( "Failed to save scene!" );
    }
    ofs.close();
}
