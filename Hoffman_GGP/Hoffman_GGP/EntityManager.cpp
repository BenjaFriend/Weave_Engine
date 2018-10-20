#include "EntityManager.h"
#include "Material.h"
#include "Mesh.h"

// Singleton requirement
static EntityManager* Instance = nullptr;


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
    // Delete each entity that has been added
    for ( auto it = EntityArray.begin(); it != EntityArray.end(); ++it )
    {
        delete ( *it );
    }

    EntityArray.clear();
}

const UINT EntityManager::AddEntity( Mesh* aMesh, Material* aMat )
{
    EntityArray.push_back( new Entity( aMesh, aMat ) );
    return static_cast<UINT>( EntityArray.size() );
}

void EntityManager::DeleteEntity( const UINT aEntityID )
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

Entity * EntityManager::GetEntity( const UINT aEntityID ) const
{
    return EntityArray[ aEntityID ];
}

//////////////////////////////////////////////////////////////////
// Accessors 

inline const size_t EntityManager::GetEntityCount() const
{
    return EntityArray.size();
}