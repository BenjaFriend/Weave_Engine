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

}

PhysicsManager::PhysicsManager()
{
    // Initialize any references to the rigid bodies
}

PhysicsManager::~PhysicsManager()
{
    // Remove references to any rigid bodies
}
