#include "../stdafx.h"
#include "RigidBody.h"

using namespace Physics;
using namespace DirectX;

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

void RigidBody::SetVelocity( const VEC3 & aVel )
{
    Velocity = aVel;
}

const VEC3 RigidBody::GetVelocity() const
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
