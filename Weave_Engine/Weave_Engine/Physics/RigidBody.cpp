#include "../stdafx.h"
#include "RigidBody.h"

#define MASS_SAVE_KEY   "Mass"
#define LAYER_SAVE_KEY  "PhysicsLayer"

COMPONENT_INIT( Physics::RigidBody )

using namespace Physics;

RigidBody::RigidBody( float aMass ) :
    Mass( aMass )
{
    Acceleration = { 0.f, 0.f, 0.f };
    Velocity = { 0.f, 0.f, 0.f };
}

Physics::RigidBody::RigidBody( nlohmann::json const & aInitData )
{
    Acceleration = { 0.f, 0.f, 0.f };
    Velocity = { 0.f, 0.f, 0.f };
    Mass = aInitData [ MASS_SAVE_KEY ];
    Layer = aInitData [ LAYER_SAVE_KEY ];
}

RigidBody::~RigidBody()
{
}

void RigidBody::DrawEditorGUI()
{
    ImGui::InputFloat( "Mass", &Mass );
}

void RigidBody::SaveComponentData( nlohmann::json & aCompData )
{
    aCompData [ MASS_SAVE_KEY ] = Mass;
    aCompData [ LAYER_SAVE_KEY ] = Layer;
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
