#include "stdafx.h"
#include "IScene.h"

IScene::IScene()
{
    EntityPool = new ObjectPool< Entity >( MAX_ENTITY_COUNT );
    Entity* EntityArray_Raw = EntityPool->GetRaw();
    // Initialize the entity array
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        EntityArray_Raw[ i ].SetIsInUse( false );
    }
}

IScene::~IScene()
{
    NetworkIdToEntityMap.clear();
    UnloadAllEntities( true );

    SAFE_DELETE( EntityPool );
}

void IScene::Update( float deltaTime, float totalTime )
{
    // Update all entities in the scene
    Entity* EntityArray_Raw = EntityPool->GetRaw();

    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( EntityArray_Raw[ i ].GetIsInUse() && EntityArray_Raw[ i ].GetIsActive() )
        {
            EntityArray_Raw[ i ].Update( deltaTime );
        }
    }
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
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID() );
    if ( it == NetworkIdToEntityMap.end() )
    {
        NetworkIdToEntityMap[ aEntity->GetNetworkID() ] = aEntity;
        aEntity->SetReplicationAction( EReplicationAction::ERA_Create );
    }
}

Entity * IScene::AddEntityFromfile( nlohmann::json const & aFile )
{
    Entity* newEnt = EntityPool->GetResource();

    assert( newEnt != nullptr );

    newEnt->SetIsInUse( true );
    newEnt->ConstructFromFile( aFile );

    LOG_TRACE( "Add raw entity from file! {}", newEnt->GetName() );

    return newEnt;
}

void IScene::UnloadAllEntities( bool aOverrideDestroyOnLoad )
{
    Entity* EntityArray_Raw = EntityPool->GetRaw();

    // Delete each entity that has been added
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        assert( &EntityArray_Raw[ i ] != nullptr );

        if ( EntityArray_Raw[ i ].GetIsInUse() &&
            ( EntityArray_Raw[ i ].GetIsDestroyableOnLoad() ||
                aOverrideDestroyOnLoad ) )
        {
            EntityArray_Raw[ i ].Reset();
            EntityPool->ReturnResource( i );
        }
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
    auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID() );
    if ( it != NetworkIdToEntityMap.end() )
    {
        NetworkIdToEntityMap.erase( aEntity->GetNetworkID() );
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
