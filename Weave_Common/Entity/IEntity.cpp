#include "stdafx.h"
#include "IEntity.h"

size_t IEntity::EntityCount = 0;

IEntity::IEntity()
{
    entID = EntityCount++;
}

IEntity::~IEntity()
{
    --EntityCount;
}

void IEntity::Write( OutputMemoryBitStream & inOutputStream, UINT32 inDirtyState ) const
{
    inOutputStream.Write( entID );

    ( void ) ( inDirtyState );
}

void IEntity::Read( InputMemoryBitStream & inInputStream )
{
    size_t ID = {};
    inInputStream.Read( ID );
    LOG_TRACE( "Entity Read: {}", ID );
}