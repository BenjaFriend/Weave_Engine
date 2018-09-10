#include "Entity.h"
#include "Mesh.h"

Entity::Entity(Mesh* aMesh)
	: EntityMesh (aMesh)
{
	// Set default values for position, scale and rotatoin
	Position	= DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	Scale		= DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	Rotation	= DirectX::XMFLOAT4X4();
}

// virtual destructor
Entity::~Entity()
{
	EntityMesh = nullptr;
}


////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

const Mesh * Entity::GetEntityMesh() const
{
	return EntityMesh;
}

const DirectX::XMFLOAT3 & Entity::GetPosition() const
{
	// TODO: insert return statement here
	return Position;
}

void Entity::SetPosition(const DirectX::XMFLOAT3 & aNewPos)
{
	Position = aNewPos;
}

const DirectX::XMFLOAT3 & Entity::GetScale() const
{
	return Scale;
}

void Entity::SetScale(const DirectX::XMFLOAT3 & aNewScale)
{
	Scale = aNewScale;
}

const DirectX::XMFLOAT4X4 & Entity::GetRotation() const
{
	return Rotation;
}

void Entity::SetRotation(const DirectX::XMFLOAT4X4 & aNewRot)
{
	Rotation = aNewRot;
}
