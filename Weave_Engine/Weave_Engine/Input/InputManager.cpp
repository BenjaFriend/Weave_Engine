#include "stdafx.h"
#include "InputManager.h"

using namespace Input;

// Static definitions
InputManager* InputManager::Instance;

InputManager::InputManager( IInput_Impl * impl )
{
    impl->InitBindings( InputBinds );
    Implementation = impl;

    for ( size_t i = 0; i < InputBinds.size(); ++i )
    {
        PreviousInputState.push_back( false );
    }
}

InputManager::~InputManager()
{
    if ( Implementation != nullptr )
    {
        delete Implementation;
    }
    Implementation = nullptr;
}

InputManager * InputManager::GetInstance()
{
    assert( Instance != nullptr );

    return Instance;
}

void InputManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
    }
}

void InputManager::SignalInput( InputType type )
{
    auto const & map_itr = actionListeners.find( type );
    if ( map_itr == actionListeners.end() ) return;

    map_itr->second.Dispatch();
}

void InputManager::OnMouseDown( WPARAM buttonState, int x, int y )
{
    // Test out the signaling of the system
    //SignalInput( InputType::Fire );
}

void InputManager::OnMouseUp( WPARAM buttonState, int x, int y )
{
    //SignalInput( InputType::FireReleased );
}

void InputManager::OnMouseMove( WPARAM buttonState, int x, int y )
{
    curMousePos = glm::vec2( x, y );
}

glm::vec2 InputManager::GetMousePosition() const
{
    return curMousePos;
}

void Input::InputManager::Update( float dt )
{
    for ( size_t i = 0; i < InputBinds.size(); ++i )
    {
        if ( IsKeyDown( InputBinds[ i ].InputValue ) )
        {
            if ( !PreviousInputState[ i ] )
            {
                SignalInput( InputBinds[ i ].Type );
                PreviousInputState[ i ] = true;
            }
        }
        else
        {
            PreviousInputState[ i ] = false;
        }
    }


    // Check any other specific things here
}

bool InputManager::IsKeyDown( int vKey )
{
    return GetAsyncKeyState( vKey ) & 0x80000;
}

bool InputManager::IsCKeyDown( char vKey )
{
    return GetAsyncKeyState( vKey ) & 0x80000;
}

bool Input::InputManager::IsInputTypeDown( InputType inputType )
{
    for ( size_t i = 0; i < InputBinds.size(); ++i )
    {
        if ( InputBinds[ i ].Type == inputType )
        {
            return IsKeyDown( InputBinds[ i ].InputValue );
        }
    }

    return false;
}

bool Input::InputManager::IsKeyUp( int vKey )
{
    return !IsKeyDown( vKey );
}

void Input::InputManager::OnLookDown( WPARAM buttonState, int x, int y )
{
    SignalInput( InputType::Look );
}

void Input::InputManager::OnLookUp( WPARAM buttonState, int x, int y )
{
    SignalInput( InputType::LookReleased );
}
