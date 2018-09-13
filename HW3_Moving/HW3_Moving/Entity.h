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
	Entity(Mesh * aMesh);		// TOD: Make this a smart pointer at some point
	
	/** Virtual destructor for this mesh */
	virtual ~Entity();

	/**
	* Move the entity relative to the direction it is facing 
	*
	* @param aX		new X value to add to this position
	* @param aY		new Y value to add to this position
	* @param aZ		new Z value to add to this position
	*/
	void MoveRelative(const float aX, const float aY, const float aZ);

	/**
	* Adds to the position NOT relative to the direction you are facing
	*
	* @param aX		new X value to add to this position
	* @param aY		new Y value to add to this position
	* @param aZ		new Z value to add to this position
	*/
	void MoveAbsolute(const float aX, const float aY, const float aZ);

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
	const DirectX::XMFLOAT4 & GetRotation() const;

	/** 
	* Set the rotation of this entity 
	* 
	* @param aNewRot	The new rotation of this object
	*/
	void SetRotation(const DirectX::XMFLOAT4 & aNewRot);

private:

	/** This entity's mesh */
	Mesh * EntityMesh = nullptr;

	/** The current positoin of this entitiy */
	DirectX::XMFLOAT3 Position;

	/** The current Scale of this entitiy */
	DirectX::XMFLOAT3 Scale;

	/** The current rotation of this entitiy */
	DirectX::XMFLOAT4 Rotation;

};

