#pragma once

#include "../stdafx.h"

#include <vector>

/// <summary>
/// A dispatcher will be able to bind and remove 
/// listeners, and call all of those listeners when 
/// it is signaled to dispatch.
/// </summary>
/// <author>Ben Hoffman</author>
class Dispatcher
{

public:

    Dispatcher()
    {

    }

    ~Dispatcher()
    {
        // Delete any created listeners
        for ( auto itr = CurrentListeners.begin(); itr != CurrentListeners.end(); ++itr )
        {
            if ( *itr != nullptr )
                delete *itr;
        }
        CurrentListeners.clear();
    }

    /// <summary>
    /// Signal this dispatcher and call any listener functions
    /// </summary>
    void Dispatch() const
    {
        for ( auto const callback : CurrentListeners )
            ( *callback ) ( );
    }

    /// <summary>
    /// Bind a function pointer to be called on an object when this dispatcher 
    /// is signaled to start
    /// </summary>
    /// <param name="aObj">Object that the callback should be called on</param>
    /// <param name="aCallBack">function to call when this is dispatch</param>
    template <typename T>
    void BindListener( T* aObj, void ( T::*aCallBack )( ) )
    {
        IListener* newListener = new MemberListener<T>( aObj, aCallBack );
        CurrentListeners.push_back( newListener );
    }

private:

    struct IListener
    {
        virtual ~IListener() {}
        virtual void operator () () = 0;
    };

    template <typename T>
    struct MemberListener : IListener
    {
        MemberListener( T* aObj, void ( T::*aCallBack )( ) )
        {
            parentObj = aObj;
            callback = aCallBack;
        }

        ~MemberListener()
        {
            parentObj = nullptr;
            callback = nullptr;
        }

        void operator () () override
        {
            assert( parentObj != nullptr );

            return ( ( parentObj->*callback )( ) );
        }

        T* parentObj = nullptr;
        void ( T::*callback )( );
    };

    /** Vector of listeners of this dispatcher */
    std::vector<IListener*> CurrentListeners;

};