#pragma once

#include <DirectXMath.h>

namespace Physics
{
    /// <summary>
    /// A simple sphere collider
    /// </summary>
    struct SphereCollider
    {
        float Radius;
        DirectX::XMFLOAT3 Center;
    };

    /// <summary>
    /// Static class with collision calculations and other helpful
    /// physics definitions
    /// </summary>
    /// <author>Ben Hoffman</author>
    class Collisions
    {
    public:

        /// <summary>
        /// Check if the point of interest is within the bounds of 
        /// the given sphere
        /// </summary>
        /// <param name="aPoint">Point of interest</param>
        /// <param name="aSphere">Sphere collider to check against</param>
        /// <returns>True if the point is inside of the sphere's radius</returns>
        static const bool IsPointInside( const DirectX::XMFLOAT3 & aPoint, const SphereCollider & aSphere );

        /// <summary>
        /// Checks if the given point is within the bounds of the box collider
        /// </summary>
        /// <param name="aPoint">Point of interest</param>
        /// <param name="aBox">Box collider to check the point against</param>
        /// <returns>True if point is inside the box collider</returns>
        //static const bool IsPointInside( const DirectX::XMFLOAT3 & aPoint, const BoxCollider & aBox );

        /// <summary>
        /// Checks if two sphere colliders are intersecting 
        /// </summary>
        /// <param name="aSphere">First sphere of interest</param>
        /// <param name="bSphere">Second sphere of interest</param>
        /// <returns>true if these two spheres are intersecting</returns>
        static const bool Intersects( const SphereCollider & aSphere, const SphereCollider & bSphere );

        /// <summary>
        /// Checks if two box colliders are intersecting
        /// </summary>
        /// <param name="aBox">First box of interest</param>
        /// <param name="bBox">Second box of interest</param>
        /// <returns>True if the boxes are intersecting</returns>
        //static const bool Intersects( const BoxCollider & aBox, const BoxCollider & bBox );

    };

}


