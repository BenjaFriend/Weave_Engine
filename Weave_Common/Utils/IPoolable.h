#pragma once

#include "stdafx.h"
#include "ObjectPool.hpp"

/// <summary>
/// A poolable object that can be reset and used as a part of an object pool
/// to avoid dynamic allocation.
/// </summary>
/// <author>Ben Hoffman</author>
template <typename T>
class IPoolable
{
public:

    void SetOwningPool( ObjectPool< T >* aOwningPool )
    {
        assert( aOwningPool != nullptr );
        owningPool = aOwningPool;
    }

    /// <summary>
    /// Reset any data that this pooled object may need to in order to be 
    /// "clean" for the pool. 
    /// </summary>
    virtual void Reset()
    {
        assert( owningPool != nullptr );
        owningPool->ReturnResource( Pool_ID );
    }

    FORCE_INLINE const bool GetIsValid() const { return IsValid; }
    FORCE_INLINE void SetIsValid( const bool aValid ) { IsValid = aValid; }

protected:

    /** If true, then this entity has been initialized and is valid in the memory pool */
    UINT32 IsValid : 1;

    /** Pointer to the object pool that owns this entity */
    ObjectPool< T >* owningPool = nullptr;

    /** The unique ID of this pooled object */
    size_t Pool_ID = 0;

};
