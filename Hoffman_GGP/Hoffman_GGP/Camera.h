#pragma once

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4

/////////////////////////////////////////////////
// Forward Declarations


/**
* Camera class capable of rendering entities, 
* determining the view and projection matrices,
* as well as some player input.
* 
* @author Ben Hoffman
*/
class Camera
{
public:

	/** Constructor; initalize matricies */
	Camera();
	
	/** Destructor for camera class */
	~Camera();

	/**
	* Update camera matricies appropriately. 
	* 
	* @param DeltaTime		Delta Time of this frame
	*/
	void Update(float DeltaTime);

	////////////////////////////////////////////////////
	// Accessors
	////////////////////////////////////////////////////

	/** Returns camera's current position */
	const DirectX::XMFLOAT3 GetPosition() const;

	/** Returns cameras current direction */
	const DirectX::XMFLOAT3 GetDirection() const;

	/** Returns camera's current Rotation */
	const float GetXAxisRotation() const;
	
	/** Returns the cameras current rotation in the Y Axis */
	const float GetYAxisRotation() const;

	/** Returns the cameras current view matrix */
	const DirectX::XMFLOAT4X4 GetViewMatrix() const;

	/** Returns the current projection matrix */
	const DirectX::XMFLOAT4X4 GetProjectMatrix() const;

private:

	/** Current position of the camera */
	DirectX::XMFLOAT3 Position;
	
	/** Current direction of the camera */
	DirectX::XMFLOAT3 Direction;

	/** Rotation in the Y axis of this camera */
	float RotationYAxis = 0.f;

	/** Rotation in the X axis of this camera */
	float RotationXAxis = 0.f;
	
	/** Current View matrix of this camera */
	DirectX::XMFLOAT4X4 ViewMatrix;

	/** Current projection matrix of the camera */
	DirectX::XMFLOAT4X4 ProjectionMatrix;
};

