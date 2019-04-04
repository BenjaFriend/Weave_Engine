#include "stdafx.h"
#include "ServerScene.h"

ServerScene::ServerScene()
{
    LOG_TRACE( "Create a server scene!" );
}

ServerScene::~ServerScene()
{
    EntityArray.clear();
    NetworkIdToEntityMap.clear();
    LOG_TRACE( "DELETE a server scene!" );
}

void ServerScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    inOutputStream.Write( static_cast< UINT32 > ( NetworkIdToEntityMap.size() ) );

    for ( const auto & entity : NetworkIdToEntityMap )
    {
        entity.second->Write( inOutputStream, 0 );
    }
}

IEntityPtr ServerScene::AddEntity( const std::string & aName, UINT32 aID )
{
    IEntityPtr newEnt = std::make_shared<IEntity>();
    newEnt->SetName( aName );
    newEnt->SetNetworkID( aID );
    EntityArray.push_back( newEnt );

    // Add this object to the replication map
    AddReplicatedObject( newEnt.get() );

    return newEnt;
}
