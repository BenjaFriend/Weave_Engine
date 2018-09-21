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


private:

	DirectX::XMFLOAT4X4 worldMatrix;
	
	DirectX::XMFLOAT4X4 viewMatrix;

	DirectX::XMFLOAT4X4 projectionMatrix;
};

