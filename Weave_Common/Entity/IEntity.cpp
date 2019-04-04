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

void IEntity::Write( OutputMemoryBitStream & inOutputStream, UINT32 inDirtyState ) const
{
    inOutputStream.Write( NetworkID );
    // Write our dirty state
    inOutputStream.Write( DirtyState, 2 );

    if ( DirtyState & EIEntityReplicationState::EIRS_POS )
    {
        const glm::vec3 pos = EntityTransform->GetPosition();
        inOutputStream.Write( pos.x );
        inOutputStream.Write( pos.y );
        inOutputStream.Write( pos.z );
    }
    
    ( void ) ( inDirtyState );
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    // Read in dirty state of this entity
    UINT32 aDirtyState = 0;
    inInputStream.Read( aDirtyState, 2 );

    // Read in pos
    if ( aDirtyState & EIEntityReplicationState::EIRS_POS )
    {
        glm::vec3 readPos( 0.f );
        inInputStream.Read( readPos.x );
        inInputStream.Read( readPos.y );
        inInputStream.Read( readPos.z );

        EntityTransform->SetPosition( readPos );

        LOG_WARN( "New Entity Pos: x: {}  Y:{}  z:{} ", readPos.x, readPos.y, readPos.z );
    }

    // #TODO: Write out each of this entities components
}