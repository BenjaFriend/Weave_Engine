#include "Entity.h"
#include "Mesh.h"

using namespace DirectX;

Entity::Entity(Mesh* aMesh)
	: EntityMesh (aMesh)
{
	// Set default values for position, scale and rotatoin
	Position	= DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	Scale		= DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	Rotation	= DirectX::XMFLOAT4();
}

Entity::Entity(const Entity & aOther)
{	
	Position	= aOther.Position;
	Scale		= aOther.Scale;
	Rotation	= aOther.Rotation;
	EntityMesh	= aOther.EntityMesh;
}

// virtual destructor
Entity::~Entity()
{
	EntityMesh = nullptr;
}

void Entity::MoveRelative(const float aX, const float aY, const float aZ)
{
	// rotate desired movement by our rotatoin
	XMVECTOR dir = XMVector3Rotate(
		XMVectorSet(aX, aY, aZ, 0),
		XMLoadFloat4(&Rotation));

	// Add to postion and store
	XMStoreFloat3(
		&Position,
		XMLoadFloat3(&Position) + dir);
}

void Entity::MoveAbsolute(const float aX, const float aY, const float aZ)
{
	Position.x += aX;
	Position.y += aY;
	Position.z += aZ;
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

Mesh * Entity::GetEntityMesh() const
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

void Entity::SetPosition(const float aX, const float aY, const float aZ)
{
	Position.x = aX;
	Position.y = aY;
	Position.z = aZ;
}

const DirectX::XMFLOAT3 & Entity::GetScale() const
{
	return Scale;
}

void Entity::SetScale(const DirectX::XMFLOAT3 & aNewScale)
{
	Scale = aNewScale;
}

void Entity::SetScale(const float aX, const float aY, const float aZ)
{
	Scale.x = aX;
	Scale.y = aY;
	Scale.z = aZ;
}

const DirectX::XMFLOAT4 & Entity::GetRotation() const
{
	return Rotation;
}

void Entity::SetRotation(const DirectX::XMFLOAT4 & aNewRot)
{
	Rotation = aNewRot;
}
