#include "pch.h"
#include "Tank.h"
#include "Entity/IEntity.h"

Tank::Tank()
{
}


Tank::~Tank()
{
}


void Tank::Update(float deltaTime)
{
	glm::vec3 movement = velocity * deltaTime;
	GetEntity()->GetTransform()->MoveRelative(movement.x, movement.y, movement.z);
}
