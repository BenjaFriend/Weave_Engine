#pragma once

#include <vector>

#include "Collisions.h"
#include "BoxCollider.h"

namespace Physics
{
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
        static void Update( float dt );



    private:

        PhysicsManager();

        ~PhysicsManager();
    
        static PhysicsManager* Instance;

        std::vector<BoxCollider*> Colliders;

    };  // class PhysicsManager
}   // namespace Physics