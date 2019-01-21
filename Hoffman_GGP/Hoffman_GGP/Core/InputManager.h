#pragma once

#include "../stdafx.h"
#include "../Utils/Dispatcher.hpp"

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

        Use,

        Quit
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
        static void ReleaseInstance();
    
        /// <summary>
        /// Bind an action to an input type. A bound action will be called when 
        /// this type of input is detected by the input manager
        /// </summary>
        /// <param name="parentObj">Object to call the action on</param>
        /// <param name="inputListenerFunc">Function to call</param>
        /// <param name="inputType">Type of input to listen for</param>
        template<class T>
        void BindAction( T* parentObj, void ( T::*inputListenerFunc )( ), InputType inputType )
        {
            actionListeners [ inputType ].BindListener( parentObj, inputListenerFunc );
        }

        /// <summary>
        /// Check for input in the input manager and signal dispatchers as necessary
        /// </summary>
        /// <param name="dt">delta time of the frame</param>
        void Update( float dt );

        bool IsKeyDown( int vKey );

        void OnLookDown( WPARAM buttonState, int x, int y );
        void OnLookUp( WPARAM buttonState, int x, int y );

        // Windows specific input callbacks
#if defined( _WIN32 ) || defined( _WIN64 )

        void OnMouseDown( WPARAM buttonState, int x, int y );
        void OnMouseUp( WPARAM buttonState, int x, int y );
        void OnMouseMove( WPARAM buttonState, int x, int y );

#endif  // _WIN32 || _WIN64

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

        /** A map of active listeners */
        std::unordered_map<InputType, Dispatcher> actionListeners;

        /// <summary>
        /// Signal to any listeners of this input type that it should happen
        /// </summary>
        /// <param name="type">The input type</param>
        void SignalInput( InputType type );

    };  // Class InputManager

}   // namespace Input