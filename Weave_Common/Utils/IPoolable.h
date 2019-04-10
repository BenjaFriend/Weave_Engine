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
        IsInUse = false;
    }

    FORCE_INLINE const bool GetIsInUse() const { return IsInUse; }
    FORCE_INLINE void SetIsInUse( const bool aValid ) { IsInUse = aValid; }

protected:

    /** If true, then this entity has been initialized and is valid in the memory pool */
    UINT32 IsInUse : 1;

    /** Pointer to the object pool that owns this entity */
    ObjectPool< T >* owningPool = nullptr;

    /** The unique ID of this pooled object */
    size_t Pool_ID = 0;

};
