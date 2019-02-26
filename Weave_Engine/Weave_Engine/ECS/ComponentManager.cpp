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
    LOG_TRACE( "Remove all componenets!" );
    std::unordered_map<EntityID, ComponentMap>::iterator entity_itr = activeComponents.begin();
    // For every entity in the component map
    while ( entity_itr != activeComponents.end() )
    {
        ECS::ComponentMap::iterator comp_itr = entity_itr->second.begin();
        // Delete every component on this entity
        while ( comp_itr != entity_itr->second.end() )
        {
            if ( comp_itr->second != nullptr )
            {
                delete comp_itr->second;
                comp_itr->second = nullptr;
            }
                
            ++comp_itr;
        }
        entity_itr->second.clear();
        ++entity_itr;
    }

    activeComponents.clear();
    ComponentCount = 0;
}