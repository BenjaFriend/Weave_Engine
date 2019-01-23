#include "../stdafx.h"
#include "RigidBody.h"

using namespace Physics;

RigidBody::RigidBody( float aMass ) :
    Mass( aMass )
{
    Acceleration = { 0.f, 0.f, 0.f };
    Velocity = { 0.f, 0.f, 0.f };
}

RigidBody::~RigidBody()
{
}

void RigidBody::DrawEditorGUI()
{
    ImGui::InputFloat( "Mass", &Mass );
}

void RigidBody::SaveObject( nlohmann::json & aOutFile )
{
    aOutFile [ "unimplemented" ] = 0;
}

void RigidBody::ApplyForce( const glm::vec3 aForce )
{
}

void RigidBody::ApplyAcceleration()
{
}

void RigidBody::SetVelocity( const glm::vec3 & aVel )
{
    Velocity = aVel;
}

const glm::vec3 RigidBody::GetVelocity() const
{
    return Velocity;
}

const float RigidBody::GetMass() const
{
    return Mass;
}

void RigidBody::SetMass( float aMass )
{
    Mass = aMass;
}

const EPhysicsLayer Physics::RigidBody::GetPhysicsLayer() const
{
    return Layer;
}
