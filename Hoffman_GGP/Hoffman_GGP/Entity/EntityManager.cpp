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
        delete ( *it );
    }

    EntityArray.clear();
}

const Entity_ID EntityManager::AddEntity( Mesh* aMesh, Material* aMat )
{
    EntityArray.push_back( new Entity( aMesh, aMat ) );
    return ( EntityArray.size() - 1 );
}

const Entity_ID EntityManager::AddEntity( Mesh * aMesh, Material * aMat, const DirectX::XMFLOAT3 & aPos )
{
    Entity* tempEnt = new Entity( aMesh, aMat );
    tempEnt->SetPosition( aPos );
    EntityArray.push_back( tempEnt );
    return ( EntityArray.size() - 1 );
}

void EntityManager::DeleteEntity( const Entity_ID aEntityID )
{
    if ( aEntityID >= 0 && aEntityID < EntityArray.size() )
    {
        Entity* tempEnt = EntityArray[ aEntityID ];
        // Remove the entity from the vector
        EntityArray.erase( EntityArray.begin() + aEntityID );
        // Free the memory of that entity
        delete EntityArray[ aEntityID ];
    }
}

Entity * EntityManager::GetEntity( const Entity_ID aEntityID ) const
{
    assert( aEntityID >= 0 && aEntityID < EntityArray.size() );

    return EntityArray[ aEntityID ];
}

//////////////////////////////////////////////////////////////////
// Accessors 

const size_t EntityManager::GetEntityCount() const
{
    return EntityArray.size();
}