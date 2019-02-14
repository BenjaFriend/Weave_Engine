#pragma once

#include "../ECS/Component.h"
#include "PhysicsManager.h"

namespace Physics
{
    enum EPhysicsLayer
    {
        MOVEABLE,
        STATIC
    };

    class RigidBody : public ECS::Component<RigidBody>
    {
    public:

        COMP_NAME( RigidBody );

        RigidBody( float aMass );

        ~RigidBody();

        virtual void DrawEditorGUI() override;

        /// <summary>
        /// Apply a force to this entity
        /// </summary>
        /// <param name="aForce">The force to apply</param>
        void ApplyForce( const glm::vec3 aForce );

        /// <summary>
        /// Apply the current acceleration to the velocity and set acceleration to 0
        /// </summary>
        void ApplyAcceleration();

        ////////////////////////////////////////////////////
        // Accessors
        ////////////////////////////////////////////////////

        void SetVelocity( const glm::vec3& aVel );

        const glm::vec3 GetVelocity() const;

        const float GetMass() const;

        void SetMass( const float aMass );

        const EPhysicsLayer GetPhysicsLayer() const;

    protected:

        virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

    private:

        /** The mass of this object */
        float Mass = 1.0f;

        glm::vec3 Velocity;

        glm::vec3 Acceleration;

        EPhysicsLayer Layer;
    };

}   // namespace Physics