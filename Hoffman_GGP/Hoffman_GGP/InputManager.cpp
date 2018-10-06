#include "InputManager.h"

// Singleton requirement
InputManager* InputManager::Instance;

InputManager::InputManager()
{
    printf( "Input Manager ctor\n" );
}

InputManager::~InputManager()
{
    // Destruct any pointers or anything we need to do here
    printf( "Input Manager dtor\n" );
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

bool InputManager::IsAsyncKeyDown( int vKey )
{
    // TODO: Replace with 
    return GetAsyncKeyState( vKey ) & 0x80000;
}
