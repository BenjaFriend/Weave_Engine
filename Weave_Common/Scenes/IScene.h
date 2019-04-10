#pragma once

#include "Entity/Entity.h"
#include "MemoryBitStream.h"
#include "Utils/ObjectPool.hpp"
#include <unordered_map>

class IScene
{
public:

    IScene();

    virtual ~IScene();

    /// <summary>
    /// Update the scene(call update on all entities and their components if necessary)
    /// </summary>
    /// <param name="deltaTime">delta time</param>
    /// <param name="totalTime">Total time of the program running</param>
    virtual void Update( float deltaTime, float totalTime );

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const;
    virtual void Read( InputMemoryBitStream& inInputStream );

    void AddReplicatedObject( Entity* aEntity );

    Entity* GetReplicatedObject( INT32 aID );

    void RemoveReplicatedObject( Entity* aEntity );

    void RemoveReplicatedObject( INT32 aID );

    FORCE_INLINE const bool IsObjectReplicated( const INT32 aNetworkID )
    {
        return ( NetworkIdToEntityMap.find( aNetworkID ) != NetworkIdToEntityMap.end() );
    }

    /// <summary>
    /// Set the state of a replicated object to dirty
    /// </summary>
    /// <param name="aNetworkID">The network object to set</param>
    /// <param name="aDirtyState">Dirty state of the object</param>
    void SetDirtyState( INT32 aNetworkID, UINT32 aDirtyState );

    /// <summary>
    /// Clear the scene of entities and clear the replication map
    /// </summary>
    virtual void ResetScene();

protected:

    /** This scene's name */
    std::string SceneName = "DEFAULT_SCENE";

    /** A map of network ID's to entity pointers */
    std::unordered_map<INT32, Entity*> NetworkIdToEntityMap;

    /** The number of replicated objects in the scene */
    INT32 NumReplicatedObjects = 0;

};
