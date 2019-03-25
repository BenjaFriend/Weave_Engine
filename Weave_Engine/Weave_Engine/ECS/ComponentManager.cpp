#include "stdafx.h"
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

bool ECS::ComponentManager::AddComponent( IEntity * aEntity, nlohmann::json & aCompData )
{
    assert( aEntity != nullptr );
    const ECS::EntityID aEntityID = aEntity->GetID();

    std::string compType = aCompData [ COMP_SAVE_KEY ];
    LOG_TRACE( "Load Component: {}", compType );

    IComponent* newComp = IComponent::ReadFromFile( aCompData );

    if ( newComp == nullptr ) return false;

    newComp->owner = aEntityID;
    newComp->OwningEntity = aEntity;
    newComp->id = ComponentCount;

    ++ComponentCount;
    const ComponentTypeId CTID = newComp->GetStaticComponentTypeID();
    this->activeComponents [ aEntityID ] [ CTID ] = newComp;

    return true;
}

bool ECS::ComponentManager::AddComponentFromEditor( IEntity * aEntity, const std::string & aCompName )
{
    assert( aEntity != nullptr );
    const ECS::EntityID aEntityID = aEntity->GetID();

    IComponent* newComp = IComponent::ReadFromEditor( aCompName );

    if ( newComp == nullptr ) return false;

    newComp->owner = aEntityID;
    newComp->OwningEntity = aEntity;
    newComp->id = ComponentCount;

    ++ComponentCount;
    const ComponentTypeId CTID = newComp->GetStaticComponentTypeID();
    this->activeComponents [ aEntityID ] [ CTID ] = newComp;

    return true;
}

ECS::ComponentManager::ComponentManager()
{
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

void ComponentManager::RemoveAllEntityComponents( const EntityID aEntityID )
{
    if ( activeComponents.find( aEntityID ) == activeComponents.end() ) return;

    ECS::ComponentMap::iterator comp_itr = activeComponents [ aEntityID ].begin();
    // Delete every component on this entity
    while ( comp_itr != activeComponents [ aEntityID ].end() )
    {
        if ( comp_itr->second != nullptr )
        {
            delete comp_itr->second;
            comp_itr->second = nullptr;
            --ComponentCount;
        }

        ++comp_itr;
    }

    activeComponents [ aEntityID ].clear();
    activeComponents.erase( aEntityID );
}