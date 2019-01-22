#include "../stdafx.h"
#include "InputManager.h"

using namespace Input;

// Static definitions
InputManager* InputManager::Instance;

InputManager::InputManager( IInput_Impl * impl )
{
    impl->InitBindings( InputBinds );
    Implementation = impl;
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
    SignalInput( InputType::Fire );
}

void InputManager::OnMouseUp( WPARAM buttonState, int x, int y )
{
    SignalInput( InputType::FireReleased );
}

void InputManager::OnMouseMove( WPARAM buttonState, int x, int y )
{

}

void Input::InputManager::Update( float dt )
{
    for ( const auto & inputVal : InputBinds )
    {
        if ( IsKeyDown( inputVal.InputValue ) )
        {
            SignalInput( inputVal.Type );
        }
    }

    // Check any other specific things here
}

bool InputManager::IsKeyDown( int vKey )
{
    return GetAsyncKeyState( vKey ) & 0x80000;
}

void Input::InputManager::OnLookDown( WPARAM buttonState, int x, int y )
{
    SignalInput( InputType::Look );
}

void Input::InputManager::OnLookUp( WPARAM buttonState, int x, int y )
{
    SignalInput( InputType::LookReleased );
}
