#include "../stdafx.h"

#include "Collisions.h"

using namespace Physics;

const bool Physics::Collisions::IsPointInside( const glm::vec3 & aPoint, const SphereCollider & aSphere )
{
    float distance =
        static_cast<float>( sqrt(
        ( aPoint.x - aSphere.Center.x ) * ( aPoint.x - aSphere.Center.x ) +
            ( aPoint.y - aSphere.Center.y ) * ( aPoint.y - aSphere.Center.y ) +
            ( aPoint.z - aSphere.Center.z ) * ( aPoint.z - aSphere.Center.z ) ) );

    return distance < aSphere.Radius;
}

const bool Physics::Collisions::Intersects( const SphereCollider & aSphere, const SphereCollider & bSphere )
{
    // we are using multiplications because it's faster than calling Math.pow
    float distance = static_cast<float>( sqrt(
        ( aSphere.Center.x - bSphere.Center.x ) * ( aSphere.Center.x - bSphere.Center.x ) +
        ( aSphere.Center.y - bSphere.Center.y ) * ( aSphere.Center.y - bSphere.Center.y ) +
        ( aSphere.Center.z - bSphere.Center.z ) * ( aSphere.Center.z - bSphere.Center.z ) ) );

    return distance < ( aSphere.Radius + bSphere.Radius );
}