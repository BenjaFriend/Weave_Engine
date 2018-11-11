#include "InputManager.h"

using namespace Input;

// Singleton requirement
InputManager* InputManager::Instance;

InputManager::InputManager()
{
    DEBUG_PRINT( "Input Manager ctor\n" );
}

InputManager::~InputManager()
{
    // Destruct any pointers or anything we need to do here
    // Destroy any input listeners
    auto itr = actionListeners.begin();
    for ( ; itr != actionListeners.end(); ++itr )
    {
        auto in_itr = itr->second.begin();
        for ( ; in_itr != itr->second.end(); ++in_itr )
        {
            if ( *( in_itr ) != nullptr )
            {
                delete *( in_itr );
            }
        }
    }
    DEBUG_PRINT( "Input Listeners destroyed!" );
}

InputManager * InputManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new InputManager();
    }

    return Instance;
}

void InputManager::Release()
{
    if ( Instance )
    {
        delete Instance;
    }
}

void InputManager::SignalInput( InputType type )
{
    auto const map_itr = actionListeners.find( type );
    if ( map_itr == actionListeners.end() ) { return; }

    std::vector<IListener*>::iterator vec_itr = map_itr->second.begin();

    // Why doesnt this heckin work
    for ( ; vec_itr != map_itr->second.end(); ++vec_itr )
        ( *( *vec_itr ) ) ( );
}

void InputManager::BindAxis( InputType type, input_action_func inputListenerFunc )
{
    IListener* newListener = new ListenerFunc( inputListenerFunc );

    actionListeners[ type ].push_back( newListener );
}

void InputManager::OnMouseDown( WPARAM buttonState, int x, int y )
{
    // Test out the signaling of the system
    SignalInput( InputType::Fire );
}

void InputManager::OnMouseUp( WPARAM buttonState, int x, int y )
{

}

void InputManager::OnMouseMove( WPARAM buttonState, int x, int y )
{

}

bool InputManager::IsKeyDown( int vKey )
{
    return GetAsyncKeyState( vKey ) & 0x80000;
}
