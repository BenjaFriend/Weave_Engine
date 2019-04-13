#pragma once

#include "Entity/Entity.h"
#include "MemoryBitStream.h"
#include "Utils/ObjectPool.hpp"
#include <unordered_map>

/// <summary>
/// Base scene class for a client and server. Can create entities
/// through an object pool and has a map of which entities are currently 
/// marked for replication. 
/// </summary>
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

    /// <summary>
    /// Load in an entity from some file information
    /// </summary>
    /// <param name="aFile"></param>
    /// <returns></returns>
    Entity* AddEntityFromfile( nlohmann::json const & aFile );

    /// <summary>
    /// Get this scene's name
    /// </summary>
    /// <returns>Reference to the current scene</returns>
    FORCE_INLINE const std::string & GetSceneName() const { return SceneName; }

    FORCE_INLINE void SetSceneName( std::string aName ) { SceneName = aName; }
    
    FORCE_INLINE Entity* GetEntity( size_t aID )
    {
        if ( aID < 0 || aID > MAX_ENTITY_COUNT ) return nullptr;

        return &EntityPool->GetRaw()[ aID ];
    }

    FORCE_INLINE Entity* GetEntityArray() const { return EntityPool->GetRaw(); }

protected:

    /// <summary>
    /// Removes all currently loaded entities from the array 
    /// and deletes them
    /// </summary>
    /// <param name="aOverrideDestroyOnLoad"></param>
    void UnloadAllEntities( bool aOverrideDestroyOnLoad = false );

    /** Object pool of entities */
    ObjectPool< Entity >* EntityPool = nullptr;

    /** This scene's name */
    std::string SceneName = "DEFAULT_SCENE";

    /** A map of network ID's to entity pointers */
    std::unordered_map< INT32, Entity* > NetworkIdToEntityMap;

    /** The number of replicated objects in the scene */
    INT32 NumReplicatedObjects = 0;

};
