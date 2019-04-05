#include "stdafx.h"
#include "IEntity.h"

size_t IEntity::EntityCount = 0;

IEntity::IEntity()
{
    IsActive = true;
    IsValid = false;
    IsDestroyableOnLoad = true;

    entID = EntityCount++;

    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    if ( EntityTransform == nullptr )
    {
        EntityTransform = this->AddComponent<Transform>();
    }
}

IEntity::~IEntity()
{
    --EntityCount;
}

void IEntity::Write( OutputMemoryBitStream & inOutputStream ) const
{
    inOutputStream.Write( NetworkID );
    // Write our replication action and the dirty state
    inOutputStream.Write( ReplicationAction, 2 );
    inOutputStream.Write( ReplicatedClassType, 2 );
    inOutputStream.Write( DirtyState );

    switch ( ReplicationAction )
    {
    case EReplicationAction::ERA_Create:
    {
        // If we are creating this object then we need to know it's type
        WriteUpdateAction( inOutputStream, DirtyState );
    }
    break;
    case EReplicationAction::ERA_Update:
    {
        WriteUpdateAction( inOutputStream, DirtyState );
    }
    break;
    case EReplicationAction::ERA_Destroy:
    {
        // If we are destroying this object, than we don't need to write anything else
    }
    break;
    }
}

void IEntity::WriteUpdateAction( OutputMemoryBitStream & inOutputStream, UINT32 inDirtyState ) const
{
    if ( inDirtyState & EIEntityReplicationState::EIRS_POS )
    {
        const glm::vec3 pos = EntityTransform->GetPosition();
        inOutputStream.Write( pos.x );
        inOutputStream.Write( pos.y );
        inOutputStream.Write( pos.z );
    }

}

void IEntity::ReadUpdateAction( InputMemoryBitStream & inInputStream )
{
	
    // Read in pos
    if ( DirtyState & EIEntityReplicationState::EIRS_POS )
    {
        glm::vec3 readPos( 0.f );
        inInputStream.Read( readPos.x );
        inInputStream.Read( readPos.y );
        inInputStream.Read( readPos.z );

        EntityTransform->SetPosition( readPos );
    }
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    // Read in dirty state of this entity
    inInputStream.Read( DirtyState );

    ReadUpdateAction( inInputStream );

    // #TODO: Write out each of this entities components
}