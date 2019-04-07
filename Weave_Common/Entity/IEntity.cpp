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

	const glm::vec3 rot = EntityTransform->GetRotation();
	inOutputStream.Write(rot.x);
	inOutputStream.Write(rot.y);
	inOutputStream.Write(rot.z);

    ( void ) ( inDirtyState );
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    glm::vec3 readPos = {};
	glm::vec3 curPos = EntityTransform->GetPosition();
    inInputStream.Read( readPos.x );
    inInputStream.Read( readPos.y );
    inInputStream.Read( readPos.z );

	glm::vec3 readRot = {};
	glm::vec3 curRot = EntityTransform->GetRotation();
	inInputStream.Read(readRot.x);
	inInputStream.Read(readRot.y);
	inInputStream.Read(readRot.z);

    EntityTransform->SetPosition( readPos );
	EntityTransform->SetRotation( readRot );

    // #TODO: Write out each of this entities components

    LOG_WARN( "New Entity Pos: x: {}  Y:{}  z:{} ", readPos.x, readPos.y, readPos.z );
}