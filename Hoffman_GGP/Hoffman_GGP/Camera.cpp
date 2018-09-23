#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	Position			= XMFLOAT3(0.f, 0.f, -5.f);
	ForwardDirection	= XMFLOAT3(0.f, 0.f, 1.f);		// Default forward vector of forward in the Z axis
	RotationXAxis		= 0.f;
	RotationYAxis		= 0.f;
}

Camera::~Camera()
{

}

void Camera::Update(float DeltaTime)
{	
	// Update the view matrix whenever the camera moves (for now, every frame)
	XMVECTOR pos			= XMVectorSet(Position.x, Position.y, Position.z, 0);
	XMVECTOR rot			= XMQuaternionRotationRollPitchYaw(RotationXAxis, RotationYAxis, 0.f);

	XMVECTOR forwardDir		= XMVectorSet(0, 0, 1, 0);
	XMVECTOR up				= XMVectorSet(0, 1, 0, 0);
	
	XMVECTOR CurrentRotation = XMVector3Rotate(forwardDir, rot);

	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		CurrentRotation,     // Direction the camera is looking
		up// "Up" direction in 3D space (prevents roll)
	);     
	
	// Store the view matrix 
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Get keyboard input from the player
	XMFLOAT3 AbsoluteInput = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 RelativeInput = XMFLOAT3(0.f, 0.f, 0.f);

	if (GetAsyncKeyState('W') & 0x80000)
	{
		RelativeInput.z += 1.f;
	}
	if (GetAsyncKeyState('S') & 0x80000)
	{
		RelativeInput.z -= 1.f;
	}
	if (GetAsyncKeyState('A') & 0x80000)
	{
		RelativeInput.x -= 1.f;
	}
	if (GetAsyncKeyState('D') & 0x80000)
	{
		RelativeInput.x += 1.f;
	}

	// Moving up and down (Absolute)
	if (GetAsyncKeyState('X') & 0x80000)
	{
		AbsoluteInput.y -= 1.f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x80000)
	{
		AbsoluteInput.y += 1.f;
	}

	//// rotate desired movement by our rotatoin
	//XMVECTOR dir = XMVector3Rotate(
	//	XMVectorSet(aX, aY, aZ, 0),
	//	XMLoadFloat4(&Rotation));

	//// Add to postion and store
	//XMStoreFloat3(
	//	&Position,
	//	XMLoadFloat3(&Position) + dir);

}

void Camera::UpdateProjectionMatrix(unsigned int aWidth, unsigned int aHeight)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)aWidth / aHeight,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&ProjectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
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