#pragma once

#include "stdafx.h"

#include <deque>

template < typename T >
class ObjectPool
{
public:

    ObjectPool( const size_t aSize );

    ~ObjectPool();

    FORCE_INLINE const bool IsEmpty() const { return AvailableIndecies.empty(); }

    FORCE_INLINE const bool IsFull() const { return AvailableIndecies.size() == MaxSize; }

    FORCE_INLINE const size_t GetNumberAvailableResources() const { return AvailableIndecies.size(); }

    /// <summary>
    /// Get an available resource 
    /// </summary>
    /// <returns>Pointer to an available resource</returns>
    FORCE_INLINE T* GetResource();

    /// <summary>
    /// Mark an index as available to the pool
    /// </summary>
    /// <param name="aIndex">The index to mark</param>
    FORCE_INLINE void ReturnResource( size_t aIndex );

    /// <summary>
    /// Get the raw array of this pool. 
    /// </summary>
    /// <returns>Pointer to the first element in the raw C array of this pool</returns>
    FORCE_INLINE T* GetRaw() { return &ObjectBuffer [ 0 ]; }

private:

    /** Array of objects  */
    T* ObjectBuffer = nullptr;

    /** Max size of this buffer */
    const size_t MaxSize;

    /** A deque of available indicies */
    std::deque< size_t > AvailableIndecies;

};

template<typename T>
inline ObjectPool<T>::ObjectPool( const size_t aSize )
    : MaxSize( aSize )
{
    ObjectBuffer = new T [ MaxSize ];

    for ( size_t i = 0; i < MaxSize; ++i )
    {
        AvailableIndecies.push_back( i );
    }
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
    if ( ObjectBuffer != nullptr )
    {
        delete [] ObjectBuffer;
    }

    AvailableIndecies.clear();
}

template<typename T>
FORCE_INLINE T * ObjectPool<T>::GetResource()
{
    if ( AvailableIndecies.empty() ) return nullptr;

    size_t index = AvailableIndecies.front();
    AvailableIndecies.pop_front();

    return &ObjectBuffer [ index ];
}

template<typename T>
FORCE_INLINE void ObjectPool<T>::ReturnResource( size_t aIndex )
{
    assert( aIndex >= 0 && aIndex < MaxSize );
    AvailableIndecies.push_back( aIndex );
}