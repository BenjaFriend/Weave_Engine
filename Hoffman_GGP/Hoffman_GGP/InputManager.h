#pragma once

#include "Windows.h"
#include <stdio.h>


/// <summary>
/// Input manager in charge of checking when input is happening
/// </summary>
class InputManager
{
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


    /// <summary>
    /// Check if the asynchronous key is down at this moment
    /// </summary>
    /// <param name="vKey">The key to check</param>
    /// <returns>True if the asynchronous key is down</returns>
    bool IsAsyncKeyDown(int vKey);

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

    // TODO: The ProcessMessage function from windows in DXCore already
    // which is where I would wanna put function pointers and stuff

};
