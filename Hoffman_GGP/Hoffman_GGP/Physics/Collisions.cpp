#include "Collisions.h"

using namespace Physics;
using namespace DirectX;

const bool Physics::Collisions::IsPointInside( const DirectX::XMFLOAT3 & aPoint, const SphereCollider & aSphere )
{
    float distance =
        static_cast<float>( sqrt(
        ( aPoint.x - aSphere.Center.x ) * ( aPoint.x - aSphere.Center.x ) +
            ( aPoint.y - aSphere.Center.y ) * ( aPoint.y - aSphere.Center.y ) +
            ( aPoint.z - aSphere.Center.z ) * ( aPoint.z - aSphere.Center.z ) ) );

    return distance < aSphere.Radius;
}

const bool Physics::Collisions::IsPointInside( const DirectX::XMFLOAT3 & aPoint, const BoxCollider & aBox )
{
    return
        ( aPoint.x >= aBox.MinX && aPoint.x <= aBox.MaxX ) &&
        ( aPoint.y >= aBox.MinY && aPoint.y <= aBox.MaxY ) &&
        ( aPoint.z >= aBox.MinZ && aPoint.z <= aBox.MaxZ );
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

const bool Physics::Collisions::Intersects( const BoxCollider & aBox, const BoxCollider & bBox )
{
    return 
        ( aBox.MinX <= bBox.MaxX && aBox.MaxX >= bBox.MinX ) &&
        ( aBox.MinY <= bBox.MaxY && aBox.MaxY >= bBox.MinY ) &&
        ( aBox.MinZ <= bBox.MaxZ && aBox.MaxZ >= bBox.MinZ );
}
