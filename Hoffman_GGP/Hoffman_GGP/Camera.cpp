#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	Position			= XMFLOAT3(0.f, 0.f, -5.f);
	ForwardDirection	= XMFLOAT3(0.f, 0.f, 1.f);		// Default forward vector of forward in the Z axis
	RelativeInput		= XMFLOAT3(0.f, 0.f, -5.f);

	RotationXAxis		= -85.f;
	RotationYAxis		= -85.f;
}

Camera::~Camera()
{

}

void Camera::Update(const float aDeltaTime)
{	
	RelativeInput.x = 0.f;
	RelativeInput.z = 0.f;
	RelativeInput.y = 0.f;

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
		RelativeInput.y -= 1.f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x80000)
	{
		RelativeInput.y += 1.f;
	}

	// Scale keyboard input by delta time
	RelativeInput.x *= aDeltaTime;
	RelativeInput.y *= aDeltaTime;
	RelativeInput.z *= aDeltaTime;

	Position.y += RelativeInput.y;

	UpdateViewMatrix(aDeltaTime);
}

void Camera::UpdateViewMatrix(const float aDeltaTime)
{

	//XMVECTOR rot = XMQuaternionRotationRollPitchYaw(RotationXAxis, RotationYAxis, 0.f);
	XMVECTOR rot = 
		UseSouthpawRotation
			? XMQuaternionRotationRollPitchYaw(RotationXAxis, RotationYAxis, 0.f) 
			: XMQuaternionRotationRollPitchYaw(RotationYAxis, RotationXAxis, 0.f);

	XMVECTOR forwardDir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	XMVECTOR CurrentRotation = XMVector3Rotate(forwardDir, rot);

	// Edit position based on the current rotation
	

	// Add to postion and store
	/*XMStoreFloat3(
		&Position,
		XMLoadFloat3(&Position) + CurrentRotation);
	*/
	
	// Update the view matrix whenever the camera moves (for now, every frame)
	XMVECTOR pos = XMVectorSet(Position.x, Position.y, Position.z, 0);
	

	XMMATRIX V = XMMatrixLookToLH(
		pos,					// The position of the "camera"
		CurrentRotation,		// Direction the camera is looking
		up						// "Up" direction in 3D space (prevents roll)
	);

	// Store the view matrix 
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}

void Camera::UpdateProjectionMatrix(const unsigned int aWidth, const unsigned int aHeight)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)aWidth / aHeight,	// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&ProjectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::UpdateMouseInput(const float aDeltaMouseX, const float aDeltaMouseY)
{
	RotationXAxis += aDeltaMouseX * HorizontalRotSpeed;
	if (RotationXAxis < -90)
	{
		RotationXAxis = -90;
	}
	else if (RotationXAxis > -85)
	{
		RotationXAxis = -85;
	}
	
	RotationYAxis += aDeltaMouseY * VerticalRotSpeed;
	if (RotationYAxis < -90)
	{
		RotationYAxis = -90;
	}
	else if (RotationYAxis > -85)
	{
		RotationYAxis = -85;
	}
	printf("Rot Values:\tX: %f Y: %f\n", RotationXAxis, RotationYAxis);
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


