#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}


////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

const DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return Position;
}

const DirectX::XMFLOAT3 Camera::GetDirection() const
{
	return Direction;
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