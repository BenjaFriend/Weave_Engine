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

void IEntity::Reset()
{
    RemoveAllComponents();
    // Reset flags
    IsValid = false;
    IsDestroyableOnLoad = true;

    // Ensure that we have a good instance of the component manager
    componentManager = ECS::ComponentManager::GetInstance();

    // Reset the transform
    EntityTransform = nullptr;    
    EntityTransform = this->AddComponent<Transform>();
}

void IEntity::Update( float dt )
{
    // Update all this entities components
    const auto & compMap = this->GetAllComponents();
    for ( auto && compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
    {
        if( compItr->second != nullptr )
            compItr->second->Update( dt );
    }
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
        const glm::vec3 & pos = EntityTransform->GetPosition();
        inOutputStream.Write( pos.x );
        inOutputStream.Write( pos.y );
        inOutputStream.Write( pos.z );
    }

    if ( inDirtyState & EIEntityReplicationState::EIRS_ROT )
    {
        const glm::vec3 & rot = EntityTransform->GetRotation();
        inOutputStream.Write( rot.x );
        inOutputStream.Write( rot.y );
        inOutputStream.Write( rot.z );
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

    if ( DirtyState & EIEntityReplicationState::EIRS_ROT )
    {
        glm::vec3 readRot( 0.0f );
        inInputStream.Read( readRot.x );
        inInputStream.Read( readRot.y );
        inInputStream.Read( readRot.z );
        EntityTransform->SetRotation( readRot );
    }
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    // Read in dirty state of this entity
    inInputStream.Read( DirtyState );

    ReadUpdateAction( inInputStream );

    // #TODO: Write out each of this entities components
}