#pragma once

#include "../stdafx.h"

#include <stdio.h>
#include <unordered_map>
#include <vector>


namespace Input
{
    // #MakeCrossPlatform
    enum InputType
    {
        Horizontal,
        Vertical,

        Fire,
        FireReleased,

        Look,
        LookReleased,

        Use
    };

    /// <summary>
    /// Input manager in charge of checking when input is happening
    /// </summary>
    class InputManager
    {
        typedef void( *input_action_func )( );

    public:

        /// <summary>
        /// Getter for the InputManager instance
        /// </summary>
        /// <returns>Singleton instance of the input manager</returns>
        static InputManager* GetInstance();

        /// <summary>
        ///  Release the instance of the input manager
        /// </summary>
        static void Release();


        void BindAction( input_action_func inputListenerFunc, InputType type );

        template<class T>
        void BindAction( T* parentObj, void ( T::*inputListenerFunc )( ), InputType type)
        {
            IListener* newListener = new ListenerMember<T>( parentObj, inputListenerFunc );

            actionListeners[ type ].push_back( newListener );
        }

        bool IsKeyDown( int vKey );

        void OnLookDown( WPARAM buttonState, int x, int y );
        void OnLookUp( WPARAM buttonState, int x, int y );

        // Windows specific input callbacks
#if defined(_WIN32) || defined(_WIN64)

        void OnMouseDown( WPARAM buttonState, int x, int y );
        void OnMouseUp( WPARAM buttonState, int x, int y );
        void OnMouseMove( WPARAM buttonState, int x, int y );

#endif

    private:

        /// <summary>
        /// Private constructor so that no objects can be created
        /// </summary>
        InputManager();

        /// <summary>
        /// Destructor for the input manager
        /// </summary>
        ~InputManager();

        /** The instance of the InputManager */
        static InputManager* Instance;

        /// <summary>
        /// Signal to any listeners of this input type that it should happen
        /// </summary>
        /// <param name="type">The input type</param>
        void SignalInput( InputType type );

        ///////////////////////////////////////////////////////
        // Listener definitions 

        struct IListener
        {
            virtual ~IListener() {}
            virtual void operator () () = 0;
        };

        struct ListenerFunc : IListener
        {
            ListenerFunc( input_action_func aFunc_ptr )
                : func_ptr( aFunc_ptr )
            {
            }

            virtual void operator () () override
            {
                return ( func_ptr() );
            }

            /** The function pointer for this input action to invoke */
            input_action_func func_ptr;
        };

        template <class T>
        struct ListenerMember : IListener
        {
            ListenerMember( T* aParent, void ( T::*f )( ) )
                : parentObj( aParent ), func_ptr( f )
            {
            }

            virtual void operator () () override
            {
                assert( parentObj != nullptr );

                return ( ( parentObj->*func_ptr )( ) );
            }

            /** the object to invoke the function pointer on */
            T* parentObj;

            /** The function pointer to call when we invoke this function */
            void ( T::*func_ptr )( );
        };

        /** A map of active listeners */
        std::unordered_map<InputType, std::vector<IListener*>> actionListeners;


    };  // Class InputManager

}   // namespace Input