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
	* 
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

	/** Set the position of this entity */
	void SetPosition(const DirectX::XMFLOAT3 & aNewPos);

	/** Return's this entity's current scale */
	const DirectX::XMFLOAT3 & GetScale() const;

	/** Set the position of htis entity */
	void SetScale(const DirectX::XMFLOAT3 & aNewScale);

	/** Return's this entity's current rotation */
	const DirectX::XMFLOAT4X4 & GetRotation() const;

	/** Set the rotation of this entity */
	void SetRotation(const DirectX::XMFLOAT4X4 & aNewRot);

private:

	/** This entity's mesh */
	Mesh * EntityMesh = nullptr;

	/** The current positoin of this entitiy */
	DirectX::XMFLOAT3 Position;

	/** The current Scale of this entitiy */
	DirectX::XMFLOAT3 Scale;

	/** The current rotatoin of this entitiy */
	DirectX::XMFLOAT4X4 Rotation;

};

