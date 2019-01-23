#pragma once

namespace Physics
{
    /// <summary>
    /// A simple sphere collider
    /// </summary>
    struct SphereCollider
    {
        float Radius;
        glm::vec3 Center;
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
        static const bool IsPointInside( const glm::vec3 & aPoint, const SphereCollider & aSphere );

        /// <summary>
        /// Checks if two sphere colliders are intersecting 
        /// </summary>
        /// <param name="aSphere">First sphere of interest</param>
        /// <param name="bSphere">Second sphere of interest</param>
        /// <returns>true if these two spheres are intersecting</returns>
        static const bool Intersects( const SphereCollider & aSphere, const SphereCollider & bSphere );


    };

}


