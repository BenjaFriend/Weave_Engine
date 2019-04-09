#include "stdafx.h"
#include "ServerScene.h"

ServerScene::ServerScene()
{
    LOG_TRACE( "Create a server scene!" );
    EntityPool = new ObjectPool< IEntity >( MAX_ENTITY_COUNT );

    IEntity* EntAray_Raw = EntityPool->GetRaw();

    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        EntAray_Raw[ i ].SetIsValid( false );
    }

}

ServerScene::~ServerScene()
{
    EntityArray.clear();
    NetworkIdToEntityMap.clear();

    SAFE_DELETE( EntityPool );
    LOG_TRACE( "DELETE a server scene!" );
}

void ServerScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    inOutputStream.Write( static_cast< UINT32 > ( NetworkIdToEntityMap.size() ) );

    // #TODO Have a replication manager to only write dirty states out
    // and what actions to take on the given object

    for ( const auto & entity : NetworkIdToEntityMap )
    {
        // #TODO Read in the class ID for this object
        entity.second->Write( inOutputStream );
        // Reset our replication action
        entity.second->SetReplicationAction( EReplicationAction::ERA_Update );
    }
}

void ServerScene::Update( float deltaTime, float totalTime )
{
    IEntity* EntityArray_Raw = EntityPool->GetRaw();

    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( EntityArray_Raw[ i ].GetIsValid() && EntityArray_Raw[ i ].GetIsActive() )
        {
            EntityArray_Raw[ i ].Update( deltaTime );
        }
    }
}

IEntity* ServerScene::AddEntity( const std::string & aName, UINT32 aID, const EReplicatedClassType aClassType)
{
    IEntity* newEnt = EntityPool->GetResource();

    assert( newEnt != nullptr );

    newEnt->SetName( aName );
    newEnt->SetNetworkID( aID );
    
    //EntityArray.push_back( newEnt );
    
    // Setup replication actions
    newEnt->SetReplicationAction( EReplicationAction::ERA_Create );
    newEnt->SetDirtyState( IEntity::EIEntityReplicationState::EIRS_AllState );
    newEnt->SetReplicationClassType( aClassType ); 
    
    // Add this object to the replication map
    AddReplicatedObject( newEnt );

    return newEnt;
}
