#pragma once

#include "stdafx.h"
#include "MemoryBitStream.h"
#include "Transform.h"

#include <memory>

class IEntity
{
public:

    IEntity();

    virtual ~IEntity();

    /// <summary>
    /// If an entity is destroyable on load, then it will be deleted during a 
    /// scene change. If not, then it will remain persistent throughout scenes
    /// </summary>
    /// <returns>True if destroyable</returns>
    FORCE_INLINE const bool GetIsDestroyableOnLoad() const { return IsDestroyableOnLoad; }

    FORCE_INLINE void SetIsDestroyableOnLoad( const bool aVal ) { IsDestroyableOnLoad = aVal; }

    FORCE_INLINE const bool GetIsValid() const { return IsValid; }

    /// <summary>
    /// Sets if this entity is active or not
    /// </summary>
    /// <param name="aStatus">True if active, false if in-active</param>
    FORCE_INLINE void SetIsActive( const bool aStatus ) { IsActive = aStatus; }

    /// <summary>
    /// Get if this entity is active or not
    /// </summary>
    /// <returns>True if active, false if in-active</returns>
    FORCE_INLINE const bool GetIsActive() const { return IsActive; }

    /// <summary>
    /// Get this entity's name
    /// </summary>
    /// <returns>Reference to the name of this entity</returns>
    FORCE_INLINE const std::string & GetName() const { return Name; }

    /// <summary>
    /// Set the name of this entity
    /// </summary>
    /// <param name="newName">The new name of this entity</param>
    FORCE_INLINE void SetName( std::string newName ) { Name = newName; }

    FORCE_INLINE void SetIsValid( const bool aValid ) { IsValid = aValid; }

    FORCE_INLINE const size_t GetID() const { return this->entID; }

    virtual class Entity* GetAsEntity() { return nullptr; }

    /// <summary>
    /// Write this component to a replicated bit stream
    /// </summary>
    /// <param name="inOutputStream"></param>
    /// <param name="inDirtyState"></param>
    virtual void Write( OutputMemoryBitStream & inOutputStream, UINT32 inDirtyState ) const;

    /// <summary>
    /// Read this component from a bit stream
    /// </summary>
    /// <param name="inInputStream"></param>
    virtual void Read( InputMemoryBitStream & inInputStream );

protected:

    static size_t EntityCount;

    /** The unique ID of this entity */
    size_t entID;

    /** Flag for if this entity is active or not */
    UINT32 IsActive : 1;

    /** If true, then this entity will get destroyed when  */
    UINT32 IsDestroyableOnLoad : 1;

    /** If true, then this entity has been initialized and is valid in the memory pool */
    UINT32 IsValid : 1;

    /** The name of this object */
    std::string Name = "Default Entity";
};

// Use smart pointers for the client proxy to have safer exits
typedef std::shared_ptr< IEntity >	IEntityPtr;