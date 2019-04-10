#include "stdafx.h"
#include "IScene.h"

IScene::IScene( )
{

}

IScene::~IScene( )
{
}

void IScene::Update(float deltaTime, float totalTime)
{
	// Update all entities in the scene

}

void IScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    ( void ) inOutputStream;
    ( void ) inDirtyState;
}

void IScene::Read( InputMemoryBitStream & inInputStream )
{
    ( void ) inInputStream;
}

void IScene::AddReplicatedObject( Entity* aEntity )
{
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID( ) );
    if ( it == NetworkIdToEntityMap.end( ) )
    {
        NetworkIdToEntityMap[ aEntity->GetNetworkID( ) ] = aEntity;
		aEntity->SetReplicationAction( EReplicationAction::ERA_Create );
    }
}

Entity* IScene::GetReplicatedObject( INT32 aID )
{
    if ( IsObjectReplicated( aID ) )
    {
        return NetworkIdToEntityMap[ aID ];
    }
    else
    {
        return nullptr;
    }
}

void IScene::RemoveReplicatedObject( Entity* aEntity )
{
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID( ) );
    if ( it != NetworkIdToEntityMap.end( ) )
    {
        NetworkIdToEntityMap.erase( aEntity->GetNetworkID( ) );
    }
}

void IScene::RemoveReplicatedObject( INT32 aID )
{
    // this will break if the entity ID doesnt exist
    NetworkIdToEntityMap.erase( aID );
}

void IScene::SetDirtyState( INT32 aNetworkID, UINT32 aDirtyState )
{
    NetworkIdToEntityMap[ aNetworkID ]->SetDirtyState( aDirtyState );
}

void IScene::ResetScene()
{
    NetworkIdToEntityMap.clear();
}
