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
}
/*
void RigidBody::ApplyAcceleration()
{
    XMVECTOR curAcceleration = XMLoadFloat3( &Acceleration );

    // Apply the acceleration to the velocity
    XMStoreFloat3(
        &Velocity,
        XMLoadFloat3( &Velocity ) + curAcceleration );

    // Add to position and store
    XMStoreFloat3(
        &Position,
        XMLoadFloat3( &Position ) + XMLoadFloat3( &Velocity ) );

    XMStoreFloat3( &Acceleration, curAcceleration * 0.f );
}

void RigidBody::ApplyForce( const VEC3 aForce )
{
    XMVECTOR force = XMLoadFloat3( &aForce );
    // Scale the vector by the mass
    force /= Mass;

    // Add to position and store
    XMStoreFloat3(
        &Acceleration,
        XMLoadFloat3( &Acceleration ) + force );
}*/

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
