#include "../stdafx.h"

#include "EntityManager.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/Mesh.h"

// Singleton requirement
EntityManager* EntityManager::Instance = nullptr;

EntityManager* EntityManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new EntityManager();
    }
    return Instance;
}

void EntityManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

EntityManager::EntityManager()
{
    SceneManagement::SceneManager* sceneMan = SceneManagement::SceneManager::GetInstance();
    
    assert( sceneMan != nullptr );

    sceneMan->OnSceneUnload().BindListener( 
        this, 
        ( &EntityManager::UnloadAllEntities ) 
    );
}

EntityManager::~EntityManager()
{
    UnloadAllEntities();
}

void EntityManager::UnloadAllEntities()
{
    // Delete each entity that has been added
    for ( auto it = EntityArray.begin(); it != EntityArray.end(); ++it )
    {
        if( *it != nullptr )
            delete ( *it );
    }

    EntityArray.clear();
}

Entity * EntityManager::AddEntity( std::string aName )
{
    Entity* tempEnt = new Entity( aName );
    EntityArray.push_back( tempEnt );
    return tempEnt;
}

Entity * EntityManager::AddEntity( std::string aName, glm::vec3 aPos )
{
    Entity* tempEnt = new Entity( aName, aPos );

    EntityArray.push_back( tempEnt );
    return tempEnt;
}

Entity * EntityManager::AddEntityFromfile( nlohmann::json const & aFile )
{
    Entity* tempEnt = new Entity( aFile );
    EntityArray.push_back( tempEnt );
    return tempEnt;
}

void EntityManager::DeleteEntity( Entity * aEntity )
{
    if ( aEntity == nullptr ) return;
    // Remove this entity from the vector
    for ( size_t i = 0; i < EntityArray.size(); ++i )
    {
        if ( EntityArray [ i ] == aEntity )
        {
            EntityArray.erase( EntityArray.begin() + i );
        }
    }

    delete aEntity;
    aEntity = nullptr;
}

//////////////////////////////////////////////////////////////////
// Accessors 

const size_t EntityManager::GetEntityCount() const
{
    return EntityArray.size();
}