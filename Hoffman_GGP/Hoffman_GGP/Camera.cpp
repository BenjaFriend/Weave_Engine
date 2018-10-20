#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
    Position = XMFLOAT3( 0.f, 0.f, -5.f );
    ForwardDirection = XMFLOAT3( 0.f, 0.f, 1.f );		// Default forward vector of forward in the Z axis
    RelativeInput = XMFLOAT3( 0.f, 0.f, -5.f );

    RotationXAxis = -85.f;
    RotationYAxis = -85.f;
}

Camera::~Camera()
{

}

void Camera::Update( const float aDeltaTime )
{
    RelativeInput.x = 0.f;
    RelativeInput.z = 0.f;
    RelativeInput.y = 0.f;

    InputManager* input = InputManager::GetInstance();


    if ( input->IsAsyncKeyDown( 'W' ) )
    {
        RelativeInput.z += 1.f;
    }
    if ( input->IsAsyncKeyDown( 'S' ) )
    {
        RelativeInput.z -= 1.f;
    }
    if ( input->IsAsyncKeyDown( 'A' ) )
    {
        RelativeInput.x -= 1.f;
    }
    if ( input->IsAsyncKeyDown( 'D' ) )
    {
        RelativeInput.x += 1.f;
    }

    // Moving up and down (Absolute)
    if ( GetAsyncKeyState( 'X' ) & 0x80000 )
    {
        RelativeInput.y -= 1.f;
    }
    if ( GetAsyncKeyState( VK_SPACE ) & 0x80000 )
    {
        RelativeInput.y += 1.f;
    }

    // Scale keyboard input by delta time
    RelativeInput.x *= aDeltaTime;
    RelativeInput.y *= aDeltaTime;
    RelativeInput.z *= aDeltaTime;

    Position.y += RelativeInput.y;

    UpdateViewMatrix( aDeltaTime );
}

void Camera::SetDoRotation( bool aDoRot )
{
    DoRotation = aDoRot;
}

const bool Camera::GetDoRotation() const
{
    return DoRotation;
}

void Camera::UpdateViewMatrix( const float aDeltaTime )
{
    // Check if we want to invert the rotation axis (southpaw) or not
    XMVECTOR rot =
        UseSouthpawRotation
        ? XMQuaternionRotationRollPitchYaw( RotationXAxis, RotationYAxis, 0.f )
        : XMQuaternionRotationRollPitchYaw( RotationYAxis, RotationXAxis, 0.f );

    XMVECTOR forwardDir = XMVectorSet( 0, 0, 1, 0 );
    XMVECTOR up = XMVectorSet( 0, 1, 0, 0 );


    XMVECTOR CurrentRotation = XMVector3Rotate( forwardDir, rot );
    // Calculate the direction we should be going left and right
    XMVECTOR left = XMVector3Cross( CurrentRotation, up );

    // Update the view matrix whenever the camera moves (for now, every frame)
    XMVECTOR pos = XMVectorSet( Position.x, Position.y, Position.z, 0 );

    pos += CurrentRotation * RelativeInput.z;
    pos -= left * RelativeInput.x;

    XMMATRIX V = XMMatrixLookToLH(
        pos,					// The position of the "camera"
        CurrentRotation,		// Direction the camera is looking
        up						// "Up" direction in 3D space (prevents roll)
    );

    // Store the view matrix 
    XMStoreFloat4x4( &ViewMatrix, XMMatrixTranspose( V ) ); // Transpose for HLSL!
    // Store our position that may have changed from input
    XMStoreFloat3( &Position, pos );
}

void Camera::UpdateProjectionMatrix( const unsigned int aWidth, const unsigned int aHeight )
{
    XMMATRIX P = XMMatrixPerspectiveFovLH(
        0.25f * 3.1415926535f,		// Field of View Angle
        (float) aWidth / aHeight,	// Aspect ratio
        0.1f,						// Near clip plane distance
        100.0f );					// Far clip plane distance
    XMStoreFloat4x4( &ProjectionMatrix, XMMatrixTranspose( P ) ); // Transpose for HLSL!
}

void Camera::UpdateMouseInput( const long aDeltaMouseX, const long aDeltaMouseY )
{
    if ( !DoRotation ) return;

    RotationXAxis += aDeltaMouseX * HorizontalRotSpeed;	// HorizontalRotSpeed = 0.005f
    if ( RotationXAxis < -90 )
    {
        RotationXAxis = -90;
    }
    else if ( RotationXAxis > -80 )
    {
        RotationXAxis = -85;
    }

    RotationYAxis += aDeltaMouseY * VerticalRotSpeed;	// VerticalRotSpeed = 0.005f
    if ( RotationYAxis < -90 )
    {
        RotationYAxis = -90;
    }
    else if ( RotationYAxis > -80 )
    {
        RotationYAxis = -85;
    }
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

const DirectX::XMFLOAT3 Camera::GetPosition() const
{
    return Position;
}

const DirectX::XMFLOAT3 Camera::GetForwardDirection() const
{
    return ForwardDirection;
}

const float Camera::GetXAxisRotation() const
{
    return RotationXAxis;
}

const float Camera::GetYAxisRotation() const
{
    return RotationYAxis;
}

const DirectX::XMFLOAT4X4 Camera::GetViewMatrix() const
{
    return ViewMatrix;
}

const DirectX::XMFLOAT4X4 Camera::GetProjectMatrix() const
{
    return ProjectionMatrix;
}

const float Camera::GetHorizontalRotSpeed() const
{
    return HorizontalRotSpeed;
}

const float Camera::GetVerticalRotSpeed() const
{
    return VerticalRotSpeed;
}


