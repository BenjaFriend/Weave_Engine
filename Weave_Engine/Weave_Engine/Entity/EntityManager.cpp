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

    Dispatcher & dis = sceneMan->OnSceneUnload();
    dis.BindListener( this, ( &EntityManager::UnloadAllEntities ) );
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
    return EntityArray [ EntityArray.size() - 1 ];
}

Entity * EntityManager::AddEntity( std::string aName, glm::vec3 aPos )
{
    Entity* tempEnt = new Entity( aName, aPos );

    EntityArray.push_back( tempEnt );
    return EntityArray [ EntityArray.size() - 1 ];
}

void EntityManager::DeleteEntity( const Entity_ID aEntityID )
{
    if ( aEntityID >= 0 && aEntityID < EntityArray.size() )
    {
        Entity* tempEnt = EntityArray [ aEntityID ];
        // Remove the entity from the vector
        EntityArray.erase( EntityArray.begin() + aEntityID );
        // Free the memory of that entity
        delete EntityArray [ aEntityID ];
    }
}

//////////////////////////////////////////////////////////////////
// Accessors 

const size_t EntityManager::GetEntityCount() const
{
    return EntityArray.size();
}