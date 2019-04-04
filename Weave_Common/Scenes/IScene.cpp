#include "stdafx.h"
#include "IScene.h"

IScene::IScene( )
{

}

IScene::~IScene( )
{
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

void IScene::AddReplicatedObject( IEntity* aEntity )
{
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID( ) );
    if ( it == NetworkIdToEntityMap.end( ) )
    {
        NetworkIdToEntityMap[ aEntity->GetNetworkID( ) ] = aEntity;
    }
}

IEntity* IScene::GetReplicatedObject( INT32 aID )
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

void IScene::RemoveReplicatedObject( IEntity* aEntity )
{
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID( ) );
    if ( it != NetworkIdToEntityMap.end( ) )
    {
        NetworkIdToEntityMap.erase( aEntity->GetNetworkID( ) );
    }
}

void IScene::SetDirtyState( INT32 aNetworkID, UINT32 aDirtyState )
{
    NetworkIdToEntityMap[ aNetworkID ]->SetDirtyState( aDirtyState );
}

void IScene::ResetScene()
{
    NetworkIdToEntityMap.clear();
}
