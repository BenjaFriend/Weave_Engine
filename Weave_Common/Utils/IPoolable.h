#pragma once

#include "stdafx.h"

/// <summary>
/// A poolable object that can be reset and used as a part of an object pool
/// to avoid dynamic allocation.
/// </summary>
/// <author>Ben Hoffman</author>
class IPoolable
{
public:

    /// <summary>
    /// Reset any data that this pooled object may need to in order to be 
    /// "clean" for the pool. 
    /// </summary>
    virtual void Reset() = 0;

    FORCE_INLINE const bool GetIsValid() const { return IsValid; }
    FORCE_INLINE void SetIsValid( const bool aValid ) { IsValid = aValid; }

protected:

    /** If true, then this entity has been initialized and is valid in the memory pool */
    UINT32 IsValid : 1;

};
