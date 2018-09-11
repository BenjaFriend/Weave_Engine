#pragma once

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;

/**
* @brief Represents a game entity and their transofrmations.
* 
* @author Ben Hoffman
*/
class Entity
{

public:

	/** 
	* Constructor for this entity
	* 
	* 
	* @param aMesh	A pointer to this entity's mesh
	*/
	Entity(Mesh* aMesh);
	
	/** Virtual destructor for this mesh */
	virtual ~Entity();

	////////////////////////////////////////////////////
	// Accessors
	////////////////////////////////////////////////////

	/** Return's this entity's mesh */
	const Mesh * GetEntityMesh() const;

	/** Return's this entity's current position */
	const DirectX::XMFLOAT3 & GetPosition() const;

	/** Set the position of this entity
	*
	* @param aNewPos	The new position of this object
	*/
	void SetPosition(const DirectX::XMFLOAT3 & aNewPos);

	/** Set the position of this enetiy 
	*
	* @param aX		new X value of the position
	* @param aY		new Y value of the position
	* @param aZ		new Z value of the position
	*/
	void SetPosition(const float aX, const float aY, const float aZ);

	/** Return's this entity's current scale */
	const DirectX::XMFLOAT3 & GetScale() const;

	/**
	* Set the position of this entity 
	* 
	* @param aNewScale	The new scale of this object
	*/
	void SetScale(const DirectX::XMFLOAT3 & aNewScale);

	/** 
	* Set the scale of the this entity 
	*
	* @param aX		new X value of the scale
	* @param aY		new Y value of the scale
	* @param aZ		new Z value of the scale
	*/
	void SetScale(const float aX, const float aY, const float aZ);

	/** Return's this entity's current rotation */
	const DirectX::XMFLOAT4X4 & GetRotation() const;

	/** 
	* Set the rotation of this entity 
	* 
	* @param aNewRot	The new rotation of this object
	*/
	void SetRotation(const DirectX::XMFLOAT4X4 & aNewRot);

private:

	/** This entity's mesh */
	Mesh * EntityMesh = nullptr;

	/** The current positoin of this entitiy */
	DirectX::XMFLOAT3 Position;

	/** The current Scale of this entitiy */
	DirectX::XMFLOAT3 Scale;

	/** The current rotation of this entitiy */
	DirectX::XMFLOAT4X4 Rotation;

};

