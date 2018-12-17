#pragma once

#include <vector>

#include "BoxCollider.h"

namespace Physics
{
    class BoxCollider;

    /// <summary>
    /// A singleton for calculating all rigid body physics 
    /// on any active RigidBody components. 
    /// </summary>
    /// <author>Ben Hoffman</author>
    class PhysicsManager
    {

    public:

        /// <summary>
        /// If there is no instance of the PhysicsManager yet, 
        /// then create one.
        /// </summary>
        /// <returns>A reference to the current instance</returns>
        static PhysicsManager* GetInstance();

        /// <summary>
        /// Release the current PhysicsManager instance.
        /// Does not delete the components, but removes any
        /// references to them.
        /// </summary>
        static void ReleaseInstance();

        /// <summary>
        /// Update any rigid body components that are active
        /// </summary>
        /// <param name="dt">Delta Time</param>
        void Update( float dt );

        /// <summary>
        /// Add a box collider that needs to be calculated in physics
        /// </summary>
        /// <param name="aBox">The box collider</param>
        void AddBoxCollider( Physics::BoxCollider* aBox );

        /// <summary>
        /// Gets a reference the the colliders vector for debug drawing
        /// </summary>
        /// <returns>const reference to the colliders in the scene</returns>
        const std::vector<Physics::BoxCollider*> & GetColliders() const;

    private:

        PhysicsManager();

        ~PhysicsManager();
    
        static PhysicsManager* Instance;

        std::vector<Physics::BoxCollider*> Colliders;

    };  // class PhysicsManager

}   // namespace Physics