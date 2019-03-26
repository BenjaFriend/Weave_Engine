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

    const glm::vec3 pos = EntityTransform->GetPosition();
    inOutputStream.Write( pos.x );
    inOutputStream.Write( pos.y );
    inOutputStream.Write( pos.z );

    ( void ) ( inDirtyState );
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    //size_t ID = {};
    //inInputStream.Read( ID );

    glm::vec3 readPos = {};
    inInputStream.Read( readPos.x );
    inInputStream.Read( readPos.y );
    inInputStream.Read( readPos.z );

    EntityTransform->SetPosition( readPos );

    // #TODO: Write out each of this entities components

    LOG_WARN( "New Entity Pos: x: {}  Y:{}  z:{} ", readPos.x, readPos.y, readPos.z );
}