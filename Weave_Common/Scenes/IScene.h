#pragma once

#include "Entity/IEntity.h"
#include "MemoryBitStream.h"
#include "Utils/ObjectPool.hpp"
#include <unordered_map>


class IScene
{
public:

    IScene();

    virtual ~IScene();

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const;
    virtual void Read( InputMemoryBitStream& inInputStream );

    void AddReplicatedObject( IEntity* aEntity )
    {
        auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID() );
        if ( it == NetworkIdToEntityMap.end() )
        {
            NetworkIdToEntityMap [ aEntity->GetNetworkID() ] = aEntity;
        }
    }

    IEntity* GetReplicatedObject( INT32 aID )
    {
        if ( IsObjectReplicated( aID ) )
        {
            return NetworkIdToEntityMap [ aID ];
        }
        else
        {
            return nullptr;
        }
    }

    void RemoveReplicatedObject( IEntity* aEntity )
    {
        auto it = NetworkIdToEntityMap.find( aEntity->GetNetworkID() );
        if ( it != NetworkIdToEntityMap.end() )
        {
            NetworkIdToEntityMap.erase( aEntity->GetNetworkID() );
        }
    }

    bool IsObjectReplicated( const INT32 aNetworkID )
    {        
        return ( NetworkIdToEntityMap.find( aNetworkID ) != NetworkIdToEntityMap.end() );
    }

protected:

    /** This scene's name */
    std::string SceneName = "DEFAULT_SCENE";

    /** A map of network ID's to entity pointers */
    std::unordered_map<INT32, IEntity*> NetworkIdToEntityMap;

    /** The number of replicated objects in the scene */
    INT32 NumReplicatedObjects = 0;

};
