#include "../stdafx.h"

#include "PhysicsManager.h"

using namespace Physics;

PhysicsManager* PhysicsManager::Instance = nullptr;

PhysicsManager * PhysicsManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new PhysicsManager();
    }
    return Instance;
}

void PhysicsManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
    }
}

void PhysicsManager::Update( float dt )
{
    // Loop through any Rigid Body components
    size_t size = Colliders.size();
    for ( size_t i = 0; i < size; ++i )
    {
        for ( size_t j = 0; j < size; ++j )
        {
            // Calculate collisions
        }
    }
}

void Physics::PhysicsManager::AddBoxCollider( Physics::BoxCollider * aBox )
{
    Colliders.push_back( aBox );
}

const std::vector<Physics::BoxCollider*> & Physics::PhysicsManager::GetColliders() const
{
    return Colliders;
}

PhysicsManager::PhysicsManager()
{
    // Initialize any references to the rigid bodies
}

PhysicsManager::~PhysicsManager()
{
    // Remove references to any rigid bodies
    Colliders.clear();
}
