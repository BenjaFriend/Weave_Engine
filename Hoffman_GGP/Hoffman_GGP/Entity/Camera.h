#pragma once

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4
#include <stdio.h>
#include "../Core/InputManager.h"   // Input

/////////////////////////////////////////////////
// Forward Declarations

/// <summary>
/// Camera class capable of rendering entities, 
/// determining the view and projection matrices,
/// as well as some player input.
/// </summary>
/// <author>Ben Hoffman</author>
class Camera
{
public:

    /** Constructor; initalize matricies */
    Camera();

    /** Destructor for camera class */
    ~Camera();

    /// <summary>
    /// Update camera matrices appropriately. 
    /// </summary>
    /// <param name="aDeltaTime">Delta Time of this frame</param>
    void Update( const float aDeltaTime );

    /// <summary>
    /// Updates the projection matrix of the camera
    /// </summary>
    /// <param name="aWidth">The aspect ration width</param>
    /// <param name="aHeight">The aspect ration height</param>
    void UpdateProjectionMatrix( const unsigned int aWidth, const unsigned int aHeight );

    /// <summary>
    /// Updates the camera's rotation based on the mouse movement
    /// of the player scaled by this cameras input speed
    /// </summary>
    /// <param name="aDeltaMouseX">The delta mouse input in X axis</param>
    /// <param name="aDeltaMouseY">The delta mouse input in Y axis</param>
    void UpdateMouseInput( const long aDeltaMouseX, const long aDeltaMouseY );

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

    /** Returns camera's current position */
    const DirectX::XMFLOAT3 GetPosition() const;

    /** Returns cameras current direction */
    const DirectX::XMFLOAT3 GetForwardDirection() const;

    /** Returns camera's current Rotation */
    const float GetXAxisRotation() const;

    /** Returns the cameras current rotation in the Y Axis */
    const float GetYAxisRotation() const;

    /** Returns the cameras current view matrix */
    const DirectX::XMFLOAT4X4 GetViewMatrix() const;

    /** Returns the current projection matrix */
    const DirectX::XMFLOAT4X4 GetProjectMatrix() const;

    /** Returns the current horizontal rotation speed */
    const float GetHorizontalRotSpeed() const;

    /** Returns the current vertical rotation speed */
    const float GetVerticalRotSpeed() const;

    /// <summary>
    /// Set if the camera should be rotating
    /// </summary>
    /// <param name="aDoRot">Should rotate</param>
    void SetDoRotation( bool aDoRot );

    /** Returns if the camera is doing rotation */
    const bool GetDoRotation() const;

    /** Returns the camera's current movements speed */
    const float GetMovementSpeed() const;

    /** Set the movement speed of this camera. Default is 4 */
    void SetMovementSpeed(float aNewVal);

private:

    /** Update the view matrix of the camera */
    void UpdateViewMatrix( const float aDeltaTime );

    /** Current position of the camera */
    DirectX::XMFLOAT3 Position;

    /** Current direction of the camera */
    DirectX::XMFLOAT3 ForwardDirection;

    /** Rotation in the X axis of this camera */
    float RotationXAxis = 0.f;

    /** Rotation in the Y axis of this camera */
    float RotationYAxis = 0.f;

    /** Movement speed of the camera */
    float MovementSpeed = 4.0f;

    /** Current View matrix of this camera. Defines the viewer of the scene. */
    DirectX::XMFLOAT4X4 ViewMatrix;

    /** Current projection matrix of the camera. Defines how the 3D scene is mapped onto 2d Screen */
    DirectX::XMFLOAT4X4 ProjectionMatrix;

    /** The horizontal rotation speed of the camera */
    const float HorizontalRotSpeed = 0.005f;

    /** The vertical rotation speed of the camera */
    const float VerticalRotSpeed = 0.005f;

    /** Whether or not the user is using southpaw rotation */
    bool UseSouthpawRotation = false;

    /** Keyboard input from the player */
    DirectX::XMFLOAT3 RelativeInput;

    /** If true than the camera will rotate */
    bool DoRotation = false;

    /** Pointer to input manager */
    Input::InputManager* inputManager = nullptr;
};
