#include "../stdafx.h"
#include "ComponentManager.h"
#include "../Entity/Entity.h"

// Singleton requirement
ECS::ComponentManager* ECS::ComponentManager::instance = nullptr;

using namespace ECS;

ECS::ComponentManager* ECS::ComponentManager::GetInstance()
{
    if ( instance == nullptr )
    {
        instance = new ComponentManager();
    }
    return instance;
}

void ECS::ComponentManager::ReleaseInstance()
{
    if ( instance != nullptr )
    {
        delete instance;
        instance = nullptr;
    }
}

ECS::ComponentManager::ComponentManager()
{
    SceneManagement::SceneManager* sceneMan = SceneManagement::SceneManager::GetInstance();

    assert( sceneMan != nullptr );

    sceneMan->OnSceneUnload().BindListener(
        this,
        ( &ComponentManager::CleanupAllComponents )
    );
}

ECS::ComponentManager::~ComponentManager()
{
    CleanupAllComponents();
}

void ECS::ComponentManager::CleanupAllComponents()
{
    auto map_itr = activeComponents.begin();

    for ( ; map_itr != activeComponents.end(); ++map_itr )
    {
        if ( map_itr->second.size() == 0 ) continue;

        auto vec_itr = map_itr->second.begin();
        for ( ; vec_itr != map_itr->second.end(); ++vec_itr )
        {
            if ( ( vec_itr->second ) != nullptr )
            {
                delete vec_itr->second;
            }
        }
        
        map_itr->second.clear();
    }

    ComponentCount = 0;
}